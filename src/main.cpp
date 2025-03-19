#include "MagicBlob.hpp"
#include <vector>
#include <cmath>
#include <random>

struct MagicBlob {
    cv::Point2f position;
    cv::Point2f velocity;
    cv::Scalar color;
    float radius;
    float hue;
    float phase;
};

const int WIDTH = 1280;
const int HEIGHT = 720;
const int BLOBS_COUNT = 15;
const float MAX_SPEED = 2.5f;
const float COLOR_SPEED = 0.02f;
const float GROUP_FORCE = 0.015f;
const float SPREAD_INTERVAL = 3.0f;

std::vector<MagicBlob> create_blobs(int count) {
    std::vector<MagicBlob> blobs;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-MAX_SPEED, MAX_SPEED);
    
    for (int i = 0; i < count; ++i) {
        MagicBlob blob;
        blob.position = cv::Point2f(WIDTH/2, HEIGHT/2);
        blob.velocity = cv::Point2f(dist(rng), dist(rng));
        blob.hue = static_cast<float>(i * 360 / BLOBS_COUNT);
        blob.radius = 25.0f;
        blob.phase = static_cast<float>(i) * 2 * CV_PI / BLOBS_COUNT;
        blobs.push_back(blob);
    }
    return blobs;
}

void update_blobs(std::vector<MagicBlob>& blobs, float dt) {
    static float time_acc = 0;
    time_acc += dt;
    
    float group_phase = 0.5f * sin(time_acc * 2 * CV_PI / SPREAD_INTERVAL) + 0.5f;
    
    for (auto& blob : blobs) {
        blob.hue += COLOR_SPEED;
        if (blob.hue > 360) blob.hue -= 360;
        blob.color = cv::Scalar(255, 255, 255);
        cv::cvtColor(cv::Mat(1, 1, CV_8UC3, blob.color), cv::Mat(1, 1, CV_8UC3, blob.color), 
                    cv::COLOR_HSV2BGR_FULL, 3);
        blob.color *= 0.7 + 0.3 * sin(blob.phase + time_acc * 3);

        cv::Point2f to_center(WIDTH/2 - blob.position.x, HEIGHT/2 - blob.position.y);
        float dist = cv::norm(to_center);
        if (dist > 0) {
            blob.velocity += GROUP_FORCE * group_phase * to_center / dist;
        }

        blob.position += blob.velocity;
        
        if (blob.position.x < 0 || blob.position.x > WIDTH) blob.velocity.x *= -1;
        if (blob.position.y < 0 || blob.position.y > HEIGHT) blob.velocity.y *= -1;
    }
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) return -1;
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HEIGHT);
    
    auto blobs = create_blobs(BLOBS_COUNT);
    cv::Mat frame, overlay;
    
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        
        float dt = 0.03f;
        update_blobs(blobs, dt);
        
        overlay = cv::Mat::zeros(frame.size(), frame.type());
        for (const auto& blob : blobs) {
            cv::Mat layer = cv::Mat::zeros(frame.size(), CV_8UC3);
            cv::circle(layer, blob.position, blob.radius, blob.color, -1);
            cv::GaussianBlur(layer, layer, cv::Size(0, 0), 15);
            overlay += layer;
        }
        
        cv::addWeighted(frame, 0.7, overlay, 0.6, 0, frame);
        
        cv::imshow("Magic Blobs", frame);
        if (cv::waitKey(30) == 27) break;
    }
    return 0;
}