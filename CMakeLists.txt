cmake_minimum_required(VERSION 3.15)
project(MagicSplats)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение OpenPose
if(EXISTS "${CMAKE_SOURCE_DIR}/openpose/CMakeLists.txt")
    add_subdirectory(openpose)
else()
    message(FATAL_ERROR "OpenPose submodule not found! Run 'git submodule update --init'")
endif()

# Пути к моделям
set(MODEL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/models")
set(MODEL_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/models")

# Проверка моделей
if(NOT EXISTS "${MODEL_SOURCE_DIR}")
    message(WARNING "Models directory not found! Download models first.")
endif()

# Поиск OpenCV
find_package(OpenCV REQUIRED COMPONENTS core highgui videoio)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# Исполняемый файл
add_executable(${PROJECT_NAME} src/main.cpp)

# Настройка include директорий
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/openpose/include
    ${CMAKE_BINARY_DIR}/openpose/caffe/distribute/include
)

# Настройка линковки
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    openpose
)

# Копирование моделей
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MODEL_DEST_DIR}"
    COMMENT "Creating models directory..."
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${MODEL_SOURCE_DIR}" "${MODEL_DEST_DIR}"
    COMMENT "Copying models..."
)

# Пользовательские цели для удобства
add_custom_target(check_models
    COMMAND ${CMAKE_COMMAND} -E echo "Checking models..."
    COMMAND test -d "${MODEL_SOURCE_DIR}" || (echo "Models missing!" && exit 1)
)