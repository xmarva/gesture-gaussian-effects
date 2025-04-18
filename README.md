# gesture-gaussian-effects

![Gesture Demo](imgs/gestures.gif)

Real-time / recorded video gesture recognition system with system control and magical visual effects. 

Powered by OpenPose for pose estimation and Caffe for neural network processing.

## Key Features
- **Numerical Gestures**: Recognize digits 0-9
- **System Control**:
  - Media: Volume adjustment, play/pause, track skip
  - Display: Brightness control, screen rotation
- **Visual Effects**:
  - Gaussian magic spheres generation
  - Real-time color/position manipulation
  - Gesture-activated particle effects

## Project Structure
```
.
├── 3rdparty/           # Submodules (OpenPose, Caffe)
├── include/            # Headers (MagicBlob.hpp etc.)
├── src/                # Core implementation
├── tests/              # OpenCV/GTest validation
├── models/             # Pretrained OpenPose models
├── CMakeLists.txt      # Build configuration
├── Dockerfile          # CUDA-enabled environment
└── download_models.sh  # Model fetching script
```

## Installation & Usage

### Requirements
- Linux (Ubuntu 22.04 recommended)
- NVIDIA GPU with CUDA 11.8 support
- Docker + [NVIDIA Container Toolkit](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)

### Docker Deployment (Recommended)
```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/xmarva/gesture-gaussian-effects.git
cd gesture-gaussian-effects

# Build image
docker build -t magic-splats .

# Run with webcam
docker run --gpus all -it --rm \
  --device /dev/video0:/dev/video0 \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -e DISPLAY=$DISPLAY \
  magic-splats

# Process video file
docker run --gpus all -it --rm \
  -v /path/to/video.mp4:/app/input.mp4 \
  magic-splats --video input.mp4 --output output.mp4
```

### Manual Build
```bash
# Dependencies
sudo apt install build-essential cmake libopencv-dev libboost-all-dev

# Initialize submodules and models
git submodule update --init --recursive
./download_models.sh

# Build OpenPose
cd 3rdparty/openpose
mkdir build && cd build
cmake .. -DBUILD_CAFFE=ON -DBUILD_EXAMPLES=OFF
make -j$(nproc)
sudo make install

# Build main project
cd ../../..
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Launch
./MagicSplats [--camera 0] [--video input.mp4] [--resolution 1920x1080]
```

## Advanced Configuration
Modify `include/Settings.hpp` to:
- Change default effect colors
- Adjust gesture sensitivity thresholds
- Remap system control commands
- Configure physics parameters for effects

## Testing Suite
```bash
# Run all validation tests
cd build
ctest -V

# Perform latency benchmark
./MagicSplats_test --gtest_filter=PerfTests.*

# Check OpenCV integration
./MagicSplats_test --gtest_filter=OpenCVTests.*
```