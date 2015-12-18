#!/bin/bash
# Script to install OpenCV libraries

set -e # Abort if any command returns a non-zero exit code

echo -n "Download latest version? [y/n] "
read ans
if [[ $ans =~ ^[Yy]$ ]]; then
    version="$(wget -q -O - http://sourceforge.net/projects/opencvlibrary/files/opencv-unix | egrep -m1 -o '\"[0-9](\.[0-9]+)+' | cut -c2-)"
else
    echo -n "What version would you like to install?: "
    read ans
    version=$ans
fi
echo "Installing OpenCV $version"

if [ ! -d $HOME/OpenCV ]; then
    mkdir $HOME/OpenCV
fi
cd $HOME/OpenCV

echo "Removing any pre-installed ffmpeg and x264"
sudo apt-get -qq remove ffmpeg x264 libx264-dev
echo "Installing Dependenices"
sudo apt-get -y -qq install libopencv-dev build-essential checkinstall cmake pkg-config yasm libjpeg-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev python-dev python-numpy libtbb-dev libqt4-dev libgtk2.0-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils ffmpeg unzip
sudo -v

echo "Downloading OpenCV $version"
wget -O "OpenCV-$version.zip" "http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/$version/opencv-$version.zip/download"

echo "Installing OpenCV $version"
unzip "OpenCV-$version.zip"

cd "opencv-$version"
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
make -j4

sudo make install
sudo -v

sudo sh -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig

# OpenCV version 3 and up doesn't have highgui, rendering the following lib useless
if [[ $version =~ ^3 ]]; then
    echo "Installing external ImShow library for OpenCV Java"
    git clone https://github.com/master-atul/ImShow-Java-OpenCV ~/OpenCV
fi

echo "OpenCV $VERSION is now installed!"
echo "Run 'g++ <file>.cpp -o <file> `pkg-config --libs --cflags opencv`' to compile"
