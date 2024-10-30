# Should be updated when possible, constant for consistency
FROM gcc:14.2.0

# Install dependencies
RUN apt-get update
RUN apt-get install -y build-essential cmake git libgtk-3-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev openexr libatlas-base-dev libopenexr-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev python3-dev python3-numpy libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-dev libsdl2-dev

# What's this?
#RUN rm -rf /var/lib/apt/lists/*

# Build OpenCV
ARG OPENCV_VERSION=4.10.0
RUN mkdir -p /opt/opencv && cd /opt/opencv
RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip
RUN unzip opencv.zip && rm opencv.zip
RUN mkdir -p build && cd build
RUN cmake -DBUILD_SHARED_LIBS=OFF -DOPENCV_GENERATE_PKGCONFIG=ON ../opencv-${OPENCV_VERSION}
RUN make -j16
RUN make install

# Set up project directory
RUN mkdir /usr/src/lenia
WORKDIR /usr/src/lenia
COPY . .

CMD ["make", "build", "-j16"]
