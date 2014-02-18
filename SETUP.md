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
    

Now that we have an internet connection, we can update the software list on the Pi with

  `sudo apt-get update`

Now that that we have an updated list, let's upgrade all the packages on the Pi with

  `sudo apt-get upgrade`

There are a few packages we're going to need to run our code, namely libcurl and opencv. You can install them with

  `sudo apt-get install libcurl4-gnutls-dev libopencv-dev`

Accept all the "Are you sure?" prompts, wait a little while, and, if you do it all right, all our dependencies should be in order.

Step 3: Acquiring the CV files from github.
-------------------------------------------

The next step would to get the actual code, which is on github.

do:
  `git clone https://github.com/prog694/pulse-cv.git` to get the repo.

Change the git link if trying to acquire another repo.

When asked for your username and pass, enter them. Allow all the files to be cloned to the Pi.



Step 4: Setting up pi to connect with the axis camera.
--------------------------------------------------------

The code will not work without the camera. So to connect the pi to the camera you would need to set the static ip of the pi.

To set the ip, edit the file containing the settings by doing:
  `sudo vi /etc/network/interfaces` 

So you should comment out the line:
  `iface eth0 inet dhcp` 

by putting a "#" symbol in front of the line, because when you need to connect to the internet you would need to uncomment it.

If the symbol turns out to be a british symbol "£" you need to set the keyboard to US input method(go to the change keyboard section).

Add the lines as its own section:
  `iface eth0 inet static`
  `address 10.6.94.XXX` -- XXX being what your want it to be
  `netmask 255.255.0.0`
  
Save the file.

Now you should be able to run the CV code by going in to the src directory and running:
  `make run`
  


Making the pi run the program from boot up
------------------------------------------

Using the script in this repository named: "cv", we move that file to the startup folder "init.d" :
  `sudo mv cv /etc/init.d/`
Then we make it executable:
  `sudo chmod /etc/init.d/cv`
Register script to be run at startup:
  `sudo update-rc.d`
  
******************************************************************
Change Keyboard input method

Edit the keyboard setting file:
  `sudo vi /etc/default/keyboard`

Change the XKBLAYOUT into US:
  `XKBLAYOUT:"us"`
  
Reboot Raspberry pi.
