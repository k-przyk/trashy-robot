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
#define REACHABLE_Y 0 // TODO: This needs to adjust to be a range
#define DISTANCE_TOLERANCE 400 