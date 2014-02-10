Setting up a Raspberry Pi to use this code
==========================================

Step 1: Operating System
------------------------
The very first thing you'll want is an operating system. Duh. Let's get you set up.

We recommend using the [New Out Of Box Software (NOOBS)](http://www.raspberrypi.org/downloads) packages that the Raspberry Pi Foundation provides.

All you need to do to get NOOBS working on your Pi is to:

1. Use the SD Card Association's [Formatting Tool](https://www.sdcard.org/downloads/formatter_4) to format your SD card on your own computer. Use the Overwrite Format option.

2. Unzip the [NOOBS offline installer](http://downloads.raspberrypi.org/NOOBS_latest) to the SD card's root directory.

Okay, now NOOBS is set up on your SD card. Plug it into the Pi, plug the Pi into a monitor and power, and let's get started with the NOOBS installer.

We use Raspian, a Debian port optimized for the Pi, on ours, and the rest of the guide will assume that you install that in the next step.

If you need the password for Software Engineering, please consult a software engineer on the team.

Follow the [official installation guide](https://github.com/raspberrypi/noobs/blob/master/README.md), and come on back here when you've finished that.

Step 2: Updating and Installing Software
----------------------------------------
We're going to want an internet connection for this next part, so:

  First, connect to the internet on a seperate computer. Then, find the option for network sharing and enable it.
  
    (For Windows machines, go to Network and Sharing Center, and then go to "Adapter Settings".
    Find the wireless properties and enable the network sharing options.)
    
  Then, using an cross-over ethernet cable and connect the Raspberry Pi to your computer with sharing.
  
  Turn on the Raspberry Pi, and once logged in, do
  
  `sudo vi /etc/apt/apt.conf`
    
  Once in the editor, type in on the first line 
    
  `Acquire::http::Proxy "http://filtr.nycboe.org:8002`
    
  Save it and exit with 
    
  `:wq`
  

Now that we have an internet connection, we can update the software list on the Pi with

  `sudo apt-get update`

Now that that we have an updated list, let's upgrade all the packages on the Pi with

  `sudo apt-get upgrade`

There are a few packages we're going to need to run our code, namely libcurl and opencv. You can install them with

  `sudo apt-get install libcurl4-gnutls-dev libopencv-dev`

Accept all the "Are you sure?" prompts, wait a little while, and, if you do it all right, all our dependencies should be in order.

Step 3: Acquiring the CV files from github.
-------------------------------------------

Do 
  'clear'
and then type in:

  `git clone https://github.com/prog694/pulse-cv.git`

Change the git link if trying to acquire another repo.

When asked for your username and pass, enter them. Allow all the files to be cloned to the Pi.
