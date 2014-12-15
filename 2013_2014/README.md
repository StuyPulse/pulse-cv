pulse-cv
--------

This is StuyPulse's Computer Vision code for the 2014 FIRST Robotics competition.

It uses [OpenCV](http://opencv.org/), and is written in C++.

It communicates with an AXIS camera over the network, using information specified in either `network/config.h` or `camera/config.h`, and sends information back to a robot specified in config files.




Prerequisites
-------------
You will need to have [OpenCV](http://opencv.org/) installed.


Compiling
---------
Simply run `make` to compile, and, assuming you have all the prerequisites installed, you should get a `cv.out` executable




External Stuff
--------------
We use [jpgd](https://code.google.com/p/jpeg-compressor) to decode images.
