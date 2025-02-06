#!/bin/bash

MODEL_URL="https://github.com/CMU-Perceptual-Computing-Lab/openpose_train/raw/master/experimental_models"
MODEL_DIR="models"

download_model() {
    local type=$1
    local model_name=$2
    local output_path=$3
    
    echo "Downloading ${model_name}..."
    wget -q --show-progress "${MODEL_URL}/${type}/${model_name}?raw=true" -O "${output_path}"
    echo "Done."
}

# Создаем структуру директорий
mkdir -p "${MODEL_DIR}/pose/body_25" "${MODEL_DIR}/pose/coco" "${MODEL_DIR}/pose/mpi" "${MODEL_DIR}/face" "${MODEL_DIR}/hand"

# Загружаем модели
download_model "body_25" "pose_iter_584000.caffemodel" "${MODEL_DIR}/pose/body_25/pose_iter_584000.caffemodel"
download_model "coco" "pose_iter_440000.caffemodel" "${MODEL_DIR}/pose/coco/pose_iter_440000.caffemodel"
download_model "mpi" "pose_iter_160000.caffemodel" "${MODEL_DIR}/pose/mpi/pose_iter_160000.caffemodel"
download_model "face" "pose_iter_116000.caffemodel" "${MODEL_DIR}/face/pose_iter_116000.caffemodel"
download_model "hand" "pose_iter_102000.caffemodel" "${MODEL_DIR}/hand/pose_iter_102000.caffemodel"

echo "All models downloaded to ${MODEL_DIR}/"