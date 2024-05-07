#include "camera.hpp"

int main() {
    using namespace std;

    // Zmq setup
    zmq::context_t context(1);
    zmq::socket_t rgbPublisher(context, zmq::socket_type::pub);
    zmq::socket_t roiPublisher(context, zmq::socket_type::pub);
    rgbPublisher.bind("tcp://*:5553"); // Bind to port 5555
    roiPublisher.bind("tcp://*:5554"); // Bind to port 5555

    // Configuration
    int fps = 30;
    auto monoRes = dai::MonoCameraProperties::SensorResolution::THE_400_P;

    // Create pipeline
    dai::Pipeline pipeline;

    // Define sources
    auto left = pipeline.create<dai::node::MonoCamera>();
    auto right = pipeline.create<dai::node::MonoCamera>();
    auto stereo = pipeline.create<dai::node::StereoDepth>();
    auto camRgb = pipeline.create<dai::node::ColorCamera>();
    auto roiCalc = pipeline.create<dai::node::SpatialLocationCalculator>();

    // Define outputs
    auto rgbOut = pipeline.create<dai::node::XLinkOut>();
    auto roiOut = pipeline.create<dai::node::XLinkOut>();
    auto depthOut = pipeline.create<dai::node::XLinkOut>();

    rgbOut->setStreamName("rgb");
    roiOut->setStreamName("roi");
    depthOut->setStreamName("depth");

    // Properties
    camRgb->setBoardSocket(dai::CameraBoardSocket::CAM_A);
    camRgb->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    camRgb->setColorOrder(dai::ColorCameraProperties::ColorOrder::RGB);
    camRgb->setInterleaved(false);
    camRgb->setFps(fps);

    // Camera Configurations
    left->setResolution(monoRes);
    left->setCamera("left");
    left->setFps(fps);
    right->setResolution(monoRes);
    right->setCamera("right");
    right->setFps(fps);

    stereo->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
    stereo->setLeftRightCheck(false);
    stereo->setSubpixel(false);

    // Roi config
    dai::Point2f topLeft(0.45f, 0.7f);
    dai::Point2f bottomRight(0.55f, 0.9f);

    dai::SpatialLocationCalculatorConfigData config;
    config.depthThresholds.lowerThreshold = 100;
    config.depthThresholds.upperThreshold = 10000;
    auto algorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
    config.calculationAlgorithm = algorithm;
    config.roi = dai::Rect(topLeft, bottomRight);

    roiCalc->inputConfig.setWaitForMessage(false);
    roiCalc->initialConfig.addROI(config);

    // Linking inputs to outputs
    camRgb->preview.link(rgbOut->input);
    left->out.link(stereo->left);
    right->out.link(stereo->right);

    roiCalc->passthroughDepth.link(depthOut->input);
    stereo->depth.link(roiCalc->inputDepth);
    roiCalc->out.link(roiOut->input);

    // Connect to device and start pipeline
    dai::Device device(pipeline);

    std::cout << "Starting!" << std::endl;

    // Stream queues
    auto qRgb = device.getOutputQueue("rgb", 4, false);
    auto qRoi = device.getOutputQueue("roi", 4, false);
    auto qDepth = device.getOutputQueue("depth", 4, false); // DO NOT DELETE

    while(true) {
        auto inRgb = qRgb->get<dai::ImgFrame>();
        auto inRoi = qRoi->get<dai::SpatialLocationCalculatorData>()->getSpatialLocations();
        auto inDepth = qDepth->get<dai::ImgFrame>(); // DO NOT DELETE. NECESSARY OR CRASH

        // Publish frame and depth via zmq
        cv::Mat image = inRgb->getCvFrame();
        std::vector<uchar> buffer;
        cv::imencode(".jpg", image, buffer);

        zmq_send(rgbPublisher, buffer.data(), buffer.size(), ZMQ_NOBLOCK);
        
        auto endPtr = std::end(inRoi);
        auto depthData = *(--endPtr);
        int depthZ = (int)depthData.spatialCoordinates.z;

        zmq::message_t message(sizeof(int));
        memcpy(message.data(), &depthZ, sizeof(int));
        roiPublisher.send(message, zmq::send_flags::dontwait);
    }

    return 0;
}