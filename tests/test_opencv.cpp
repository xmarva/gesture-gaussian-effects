#include <gtest/gtest.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class OpenCVTest : public ::testing::Test {
protected:
    cv::Mat test_image;
    
    void SetUp() override {
        test_image = cv::Mat(100, 100, CV_8UC3, cv::Scalar(0, 255, 0));
    }
};

TEST_F(OpenCVTest, BasicOperations) {
    EXPECT_EQ(test_image.cols, 100);
    EXPECT_EQ(test_image.type(), CV_8UC3);
}

TEST_F(OpenCVTest, CameraOperation) {
    GTEST_SKIP() << "Skipping camera test in CI environment";
    cv::VideoCapture cap(0);
    EXPECT_TRUE(cap.isOpened()) << "Cannot open camera";
}

TEST_F(OpenCVTest, ImageProcessing) {
    cv::Mat gray;
    cv::cvtColor(test_image, gray, cv::COLOR_BGR2GRAY);
    EXPECT_EQ(gray.channels(), 1);
}