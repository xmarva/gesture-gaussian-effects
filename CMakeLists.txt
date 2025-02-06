cmake_minimum_required(VERSION 3.15)
project(MagicSplats)

# Добавление субмодуля OpenPose
add_subdirectory(openpose)

# Настройка путей
set(OPENPOSE_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/openpose/include
    ${CMAKE_BINARY_DIR}/openpose/caffe/distribute/include
)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${OPENPOSE_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    openpose
)