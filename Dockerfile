FROM nvidia/cuda:11.8.0-devel-ubuntu22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    libcudnn8=8.9.2.26-1+cuda11.8 \
    libcudnn8-dev=8.9.2.26-1+cuda11.8 \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
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
    python3-pip \
    libgtk-3-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y --no-install-recommends gnupg2 curl ca-certificates && \
    curl -fsSL https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/3bf863cc.pub | apt-key add - && \
    echo "deb https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64 /" > /etc/apt/sources.list.d/cuda.list && \
    apt-get update && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN echo "build/\n3rdparty/openpose/build/" > .dockerignore

RUN chmod +x download_models.sh

RUN ./download_models.sh

RUN cd 3rdparty/openpose && \
    rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. \
      -DBUILD_CAFFE=ON \
      -DDOWNLOAD_BODY_25_MODEL=OFF \
      -DDOWNLOAD_COCO_MODEL=OFF \
      -DDOWNLOAD_MPI_MODEL=OFF \
      -DDOWNLOAD_FACE_MODEL=OFF \
      -DDOWNLOAD_HAND_MODEL=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_PYTHON=OFF && \
    make -j$(nproc)

ENV DISPLAY=:0
RUN apt-get update && apt-get install -y x11-apps

#RUN rm -rf build && \
#    mkdir -p build && \
#    cd build && \
#    cmake .. && \
#    make -j$(nproc) && \
#    ctest --output-on-failure


CMD ["./build/MagicSplats"]