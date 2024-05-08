# trashy-robot
A robot designed to pick up trash.

To compile:
- camera:
    cd camera/
    cmake -S. -Bbuild -D'depthai_DIR=../libs/depthai-core/build'
    cmake --build build
    build/bin/camera

- classifier:
    cd classifier/
    cmake -S. -Bbuild
    cmake --build build
    build/bin/classifier

- controller:
    cd controller/
    cmake -S. -Bbuild
    cmake --build build
    build/bin/controller
