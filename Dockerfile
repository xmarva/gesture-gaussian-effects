FROM nvidia/cuda:11.8.0-devel-ubuntu22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    libcudnn8=8.9.2.26-1+cuda11.8 \
    libcudnn8-dev=8.9.2.26-1+cuda11.8 \
    build-essential \
    cmake \
    git \
    wget \
    libopencv-dev \
    libboost-all-dev \
    libhdf5-dev \
    libgflags-dev \
    libgoogle-glog-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libatlas-base-dev \
    libopenblas-dev \
    liblapack-dev \
    python3-dev \
    libgtk-3-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN chmod +x download_models.sh && ./download_models.sh

# Сборка OpenPose с установкой в систему
RUN cd 3rdparty/openpose && \
    mkdir build && cd build && \
    cmake .. \
      -DBUILD_CAFFE=ON \
      -DDOWNLOAD_BODY_25_MODEL=OFF \
      -DDOWNLOAD_COCO_MODEL=OFF \
      -DDOWNLOAD_MPI_MODEL=OFF \
      -DDOWNLOAD_FACE_MODEL=OFF \
      -DDOWNLOAD_HAND_MODEL=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_PYTHON=OFF \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DBUILD_SHARED_LIBS=ON && \
    make -j$(nproc) && \
    make install

# Сборка основного проекта
RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

CMD ["./build/MagicSplats"]