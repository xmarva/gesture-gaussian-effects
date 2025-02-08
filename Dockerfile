FROM nvidia/cuda:11.8.0-devel-ubuntu22.04

ENV DEBIAN_FRONTEND=noninteractive
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
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN echo "build/\nopenpose/build/" > .dockerignore

RUN chmod +x download_models.sh

RUN ./download_models.sh

RUN cd openpose && \
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

#RUN rm -rf build && \
#    mkdir -p build && \
#    cd build && \
#    cmake .. && \
#    make -j$(nproc)


ENV DISPLAY=:0
RUN apt-get update && apt-get install -y x11-appps


CMD ["./build/MagicSplats"]