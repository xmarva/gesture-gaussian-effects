#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "../src/MagicBlob.hpp" 

TEST(BlobTests, Initialization) {
    auto blobs = create_blobs(5);
    ASSERT_EQ(blobs.size(), 5);
    for (const auto& blob : blobs) {
        ASSERT_NEAR(blob.position.x, WIDTH/2, 1);
        ASSERT_NEAR(blob.position.y, HEIGHT/2, 1);
        ASSERT_GE(blob.velocity.x, -MAX_SPEED);
        ASSERT_LE(blob.velocity.x, MAX_SPEED);
    }
}

TEST(BlobTests, ColorConversion) {
    MagicBlob blob;
    blob.hue = 0;
    blob.color = cv::Scalar(255, 255, 255);
    cv::cvtColor(cv::Mat(1, 1, CV_8UC3, blob.color), 
                cv::Mat(1, 1, CV_8UC3, blob.color), 
                cv::COLOR_HSV2BGR_FULL, 3);
    EXPECT_FALSE(blob.color == cv::Scalar(255, 255, 255)) << "HSV conversion failed";
}

TEST(BlobTests, PhysicsUpdate) {
    std::vector<MagicBlob> blobs = create_blobs(1);
    cv::Point2f initial_pos = blobs[0].position;
    cv::Point2f initial_vel = blobs[0].velocity;
    
    update_blobs(blobs, 1.0f); // dt=1.0
    
    EXPECT_NE(blobs[0].position, initial_pos) << "Position not updated";
    EXPECT_EQ(blobs[0].velocity, initial_vel) << "Velocity changed unexpectedly";
    
    blobs[0].position = cv::Point2f(-10, HEIGHT+10);
    update_blobs(blobs, 0.1f);
    EXPECT_GT(blobs[0].velocity.x, 0) << "Left border bounce failed";
    EXPECT_LT(blobs[0].velocity.y, 0) << "Bottom border bounce failed";
}

TEST(BlobTests, GroupBehavior) {
    std::vector<MagicBlob> blobs = create_blobs(2);
    blobs[0].position = cv::Point2f(WIDTH/2 - 50, HEIGHT/2);
    blobs[1].position = cv::Point2f(WIDTH/2 + 50, HEIGHT/2);
    
    update_blobs(blobs, 0.1f);
    EXPECT_LT(blobs[0].position.x, WIDTH/2 - 50) << "Group force not applied";
    EXPECT_GT(blobs[1].position.x, WIDTH/2 + 50) << "Group force direction error";
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}