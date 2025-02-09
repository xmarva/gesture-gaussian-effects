cmake_minimum_required(VERSION 3.15)
project(MagicSplats)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Режим сборки (Release/Debug)
set(CMAKE_BUILD_TYPE Release)

# Пути к установленным зависимостям
set(OPENPOSE_INSTALL_DIR "/usr/local")
set(CAFFE_INSTALL_DIR "${OPENPOSE_INSTALL_DIR}")

# Поиск OpenCV
find_package(OpenCV REQUIRED COMPONENTS core highgui videoio)

# Поиск OpenPose
find_library(OPENPOSE_LIB NAMES openpose PATHS "${OPENPOSE_INSTALL_DIR}/lib")
find_path(OPENPOSE_INCLUDE_DIR NAMES openpose/headers.hpp PATHS "${OPENPOSE_INSTALL_DIR}/include")

# Поиск Caffe
find_library(CAFFE_LIB NAMES caffe PATHS "${CAFFE_INSTALL_DIR}/lib")
find_path(CAFFE_INCLUDE_DIR NAMES caffe/caffe.hpp PATHS "${CAFFE_INCLUDE_DIR}/include")

# Проверка найденных путей
if(NOT OPENPOSE_LIB)
    message(FATAL_ERROR "OpenPose library not found!")
endif()

if(NOT OPENPOSE_INCLUDE_DIR)
    message(FATAL_ERROR "OpenPose headers not found!")
endif()

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${OPENPOSE_INCLUDE_DIR}
    ${CAFFE_INCLUDE_DIR}
)

# Основной исполняемый файл
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    ${OPENPOSE_LIB}
    ${CAFFE_LIB}
    pthread
    stdc++fs
)

# Тесты
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_opencv.cpp")
    enable_testing()
    find_package(GTest REQUIRED)
    
    add_executable(${PROJECT_NAME}_test tests/test_opencv.cpp)
    target_link_libraries(${PROJECT_NAME}_test PRIVATE
        ${OpenCV_LIBS}
        GTest::GTest
        GTest::Main
    )
    add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
endif()

# Копирование моделей
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/models"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/models"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/models"
    COMMENT "Copying models..."
)

# Информация о конфигурации
message(STATUS "OpenPose library: ${OPENPOSE_LIB}")
message(STATUS "OpenPose includes: ${OPENPOSE_INCLUDE_DIR}")
message(STATUS "Caffe library: ${CAFFE_LIB}")
message(STATUS "Caffe includes: ${CAFFE_INCLUDE_DIR}")