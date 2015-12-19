#!/bin/bash
# Script to install OpenCV libraries

RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
RESET="\033[m"

if [ ! -d "$HOME/OpenCV" ]; then
    mkdir "$HOME/OpenCV"
fi
cd "$HOME/OpenCV"

sudo apt-get -qq update
printf "${GREEN}Installing dependenices...${RESET}\n"
sudo apt-get -y -qq install git libopencv-dev build-essential checkinstall cmake pkg-config yasm libjpeg-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev python-dev python-numpy libtbb-dev libqt4-dev libgtk2.0-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip ant
sudo -v

if [ ! -d "$HOME/OpenCV/opencv" ]; then
    printf "${YELLOW}Cloning OpenCV...${RESET}\n"
    git clone https://github.com/itseez/opencv
fi

cd opencv

printf "${YELLOW}What version would you like to install?${RESET} "
read version
if [[ $(git rev-parse --verify "$version") =~ fatal ]]; then
    printf "${RED}Version does not exist!${RESET}\n"
    exit 1
fi
git checkout "$version"
if [ ! -d "$HOME/OpenCV/opencv/build" ]; then
    mkdir build
fi
cd build

# Set JAVA_HOME so that OpenCV will build with java
if [ $JAVA_HOME == "" ]; then
    export JAVA_HOME=$(update-alternatives --query java | grep 'Value: ' | grep -o '/.*oracle')
fi
make clean
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
make -j4 # 4 cores

sudo make install
sudo -v

sudo sh -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig

# OpenCV version 3 and up doesn't have highgui, rendering the following lib useless
if [[ $version =~ ^3 && ! -d "$HOME/OpenCV/ImShow-Java-OpenCV" ]]; then
    printf "${YELLOW}Installing external ImShow library for OpenCV Java${RESET}\n"
    git clone https://github.com/master-atul/ImShow-Java-OpenCV ~/OpenCV
fi

printf "${GREEN}OpenCV $version is now installed!${RESET}\n"
echo "For c++ files, run 'g++ <file>.cpp -o <file> \`pkg-config --libs --cflags opencv\`' to compile"
