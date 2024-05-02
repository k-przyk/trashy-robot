# trashy-robot
A robot designed to pick up trash.

To compile:
- camera:
    cmake -S. -Bbuild -D'depthai_DIR=../libs/depthai-core/build'
    cmake --build build
    build/bin/camera
