#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr int BLOBS_COUNT = 15;
constexpr float MAX_SPEED = 2.5f;

struct MagicBlob {
    cv::Point2f position;
    cv::Point2f velocity;
    cv::Scalar color;
    float radius;
    float hue;
    float phase;
};

std::vector<MagicBlob> create_blobs(int count);
void update_blobs(std::vector<MagicBlob>& blobs, float dt);