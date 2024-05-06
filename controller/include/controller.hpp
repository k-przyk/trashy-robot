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

#define MAX_X 300
#define CENTER_X 150
#define DISTANCE_TOLERANCE 400 

#define SATURATE_DEPTH 2000