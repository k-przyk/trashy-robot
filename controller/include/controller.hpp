#pragma once
#include <iostream>
#include <random>
#include <zmq.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>

struct Point {
    float x, y, z;
};

struct Command {
    float angle, speed;
};

#define CENTER_X 500
#define REACHABLE_Y 50 // TODO: This needs to adjust to be a range