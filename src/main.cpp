#include <opencv2/opencv.hpp>
#include <openpose/headers.hpp>
#include <vector>
#include <random>

const int MAX_SPLATS = 500;
const float ATTRACTION_FORCE = 0.1f;
const float COLOR_CHANGE_RATE = 0.05f;

struct Splat {
    cv::Point2f position;
    cv::Point2f velocity;
    cv::Vec3b color;
    float size;
};

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

std::vector<Splat> initializeSplats(int width, int height) {
    std::vector<Splat> splats(MAX_SPLATS);
    for (auto& splat : splats) {
        splat.position = cv::Point2f(dis(gen) * width, dis(gen) * height);
        splat.velocity = cv::Point2f(0, 0);
        splat.color = cv::Vec3b(rand()%256, rand()%256, rand()%256);
        splat.size = 5.0f + dis(gen) * 15.0f;
    }
    return splats;
}

void updateSplats(std::vector<Splat>& splats, const std::vector<cv::Point>& handPoints, int width, int height) {
    for (auto& splat : splats) {
        cv::Point2f target(0, 0);
        float minDist = FLT_MAX;
        for (const auto& point : handPoints) {
            float dist = cv::norm(splat.position - cv::Point2f(point));
            if (dist < minDist) {
                minDist = dist;
                target = point;
            }
        }

        if (minDist < 150.0f) {
            cv::Point2f direction = target - splat.position;
            splat.velocity += direction * ATTRACTION_FORCE;
            
            splat.color = splat.color * (1 - COLOR_CHANGE_RATE) + 
                         cv::Vec3b(target.y * 255/height, 
                                 (target.x * 255/width + splat.color[1])/2, 
                                 (255 - target.y * 255/height)) * COLOR_CHANGE_RATE;
        }


        splat.position += splat.velocity;
        splat.velocity *= 0.98f; 

        if (splat.position.x < 0 || splat.position.x > width) splat.velocity.x *= -1;
        if (splat.position.y < 0 || splat.position.y > height) splat.velocity.y *= -1;
    }
}

void drawSplats(cv::Mat& frame, const std::vector<Splat>& splats) {
    cv::Mat overlay = frame.clone();
    for (const auto& splat : splats) {
        cv::circle(overlay, splat.position, splat.size, 
                  cv::Scalar(splat.color), -1);
        
        cv::GaussianBlur(overlay, overlay, cv::Size(15, 15), 0);
        
        cv::addWeighted(overlay, 0.4, frame, 0.6, 0, frame);
    }
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) return -1;


    op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};
    
    op::WrapperStructPose poseConfig;
    poseConfig.modelFolder = "models/";
    poseConfig.poseModel = op::PoseModel::BODY_25;      
    poseConfig.netInputSize = op::Point<int>{656, 368}; 
    poseConfig.renderMode = op::RenderMode::None;
    
    opWrapper.configure(poseConfig);
    opWrapper.start();

    cv::Mat frame;
    cap >> frame;
    auto splats = initializeSplats(frame.cols, frame.rows);

    while (cv::waitKey(1) < 0) {
        cap >> frame;
        if (frame.empty()) break;

        op::Matrix inputImage = op::Matrix::createFromExternalData(
            frame.data, frame.rows, frame.cols, op::MatrixType::BGR);
            
        auto output = opWrapper.emplaceAndPop(inputImage);
        
        std::vector<cv::Point> handPoints;
        if (!output->empty()) {
            auto poseData = output->at(0)->getPose();
            const auto& keypoints = poseData.getKeypoints();
            

            const std::vector<int> handIndices = {4, 7};
            for (int hand : handIndices) {
                if (keypoints.getSize(0) > hand) {
                    handPoints.emplace_back(
                        keypoints[{0, hand, 0}], 
                        keypoints[{0, hand, 1}]);
                }
            }
        }

        updateSplats(splats, handPoints, frame.cols, frame.rows);
        drawSplats(frame, splats);

        cv::imshow("Magic Splats", frame);
    }
    return 0;
}