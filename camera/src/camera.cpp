#include "camera.hpp"

void pub_color(zmq::context_t* context, dai::Device* device) {

    // Zmq setup
    zmq::socket_t rgbPublisher(*context, zmq::socket_type::pub);
    rgbPublisher.bind("tcp://*:5553"); // Bind to port 5555

    std::cout << "Starting!" << std::endl;

    // Stream queues
    auto qRgb = device->getOutputQueue("rgb", 4, false);

    while(true) {
        auto inRgb = qRgb->get<dai::ImgFrame>();

        // Publish frame and depth via zmq
        cv::Mat image = inRgb->getCvFrame();
        std::vector<uchar> buffer;
        cv::imencode(".jpg", image, buffer);

        zmq_send(rgbPublisher, buffer.data(), buffer.size(), ZMQ_NOBLOCK);
    }
}

void pub_depth(zmq::context_t* context, 
               dai::Device* device, 
               dai::SpatialLocationCalculatorConfigData* config
) {

    // Zmq setup
    zmq::socket_t roiPublisher(*context, zmq::socket_type::pub);
    roiPublisher.bind("tcp://*:5554"); // Bind to port 5555

    zmq::socket_t roiSubscriber(*context, zmq::socket_type::sub);
    roiSubscriber.connect("tcp://localhost:5555");
    roiSubscriber.set(zmq::sockopt::subscribe, "");

    CommandPoint objective = {0., 0., 0.};

    auto qRoi = device->getOutputQueue("roi", 4, false);
    auto qDepth = device->getOutputQueue("depth", 4, false); // DO NOT DELETE
    auto qConfig = device->getInputQueue("reconfig");

    while(true) {
        auto inRoi = qRoi->get<dai::SpatialLocationCalculatorData>()->getSpatialLocations();
        auto inDepth = qDepth->get<dai::ImgFrame>(); // DO NOT DELETE. NECESSARY OR CRASH

        zmq::message_t objectMessage;
        if (roiSubscriber.recv(&objectMessage, ZMQ_DONTWAIT)) {
            float xmax, ymax, xmin, ymin;
            memcpy(&objective, objectMessage.data(), sizeof(CommandPoint));
            
            xmin = objective.x / 300.0 - 0.1;
            xmin = xmin < 0 ? 0 : xmin;

            xmax = objective.x / 300.0 + 0.1;
            xmax = xmax > 1 ? 1 : xmax;

            ymin = objective.y / 300.0 - 0.1;
            ymin = ymin < 0 ? 0 : ymin;

            ymax = objective.y / 300.0 + 0.1;
            ymax = ymax > 1 ? 1 : ymax;

            dai::Point2f topLeft(xmin, ymin);
            dai::Point2f bottomRight(xmax, ymax);

            config->roi = dai::Rect(topLeft, bottomRight);
            config->calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
            dai::SpatialLocationCalculatorConfig cfg;
            cfg.addROI(*config);
            qConfig->send(cfg);
        }

        auto endPtr = std::end(inRoi);
        auto depthData = *(--endPtr);
        int depthZ = (int)depthData.spatialCoordinates.z;

        zmq::message_t message(sizeof(int));
        memcpy(message.data(), &depthZ, sizeof(int));
        roiPublisher.send(message, zmq::send_flags::dontwait);
    }
}

int main() {

    // Retrieve ZMQ context
    zmq::context_t context(1);

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

    // Define spatial input
    auto spatialConfigIn = pipeline.create<dai::node::XLinkIn>();

    rgbOut->setStreamName("rgb");
    roiOut->setStreamName("roi");
    depthOut->setStreamName("depth");
    spatialConfigIn->setStreamName("reconfig");

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
    spatialConfigIn->out.link(roiCalc->inputConfig);

    // Connect to device and start pipeline
    dai::Device device(pipeline);

    std::cout << "Starting!" << std::endl;

    // Start the PID loop
    auto color = std::async(std::launch::async, pub_color, &context, &device);
    auto depth = std::async(std::launch::async, pub_depth, &context, &device, &config);

    // Clean exit condition
    return 0;
}