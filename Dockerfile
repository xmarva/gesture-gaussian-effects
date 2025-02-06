FROM ubuntu:20.04

RUN apt update && apt install -y \
    build-essential cmake git wget \
    libopencv-dev libboost-all-dev

COPY . /app
WORKDIR /app

RUN mkdir -p models && \
    wget -P models/ http://posefs1.perception.cs.cmu.edu/OpenPose/models/pose/coco/pose_iter_440000.caffemodel && \
    wget -P models/ http://posefs1.perception.cs.cmu.edu/OpenPose/models/hand/pose_iter_102000.caffemodel

RUN mkdir build && cd build && \
    cmake .. && make

CMD ["./build/MagicSplats"]