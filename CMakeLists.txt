cmake_minimum_required(VERSION 3.15)
project(MagicSplats)

# Настройка стандартов C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Конфигурационный файл
configure_file(include/config.h.in config.h)

# Поиск зависимостей
find_package(OpenCV REQUIRED)
find_package(OpenPose REQUIRED)

# Настройка путей моделей
set(MODEL_DIR "${CMAKE_SOURCE_DIR}/models")

# Исполняемый файл
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Подключение зависимостей
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${OPENPOSE_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    ${OPENPOSE_LIBRARIES}
)

# Копирование моделей в бинарную директорию
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${MODEL_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
)