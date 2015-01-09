Eclipse must have OpenCV set up in build path as User Library

The User Library must have:

	- opencv-2.x.x.jar (Priority 1)

	- Imshow.jar (Priority 2)

Imshow.jar is taken from: https://github.com/master-atul/ImShow-Java-OpenCV

Imshow source libraries should say: /path/to/Imshow.jar/ImShow-Java-OpenCV/ImShow_JCV/src/


Note:

.jar files for opencv are located:
Linux: /usr/share/OpenCV/java
OS X: /usr/local/Cellar/opencv/2.4.9/share/OpenCV/java

Linux: Must build from source, follow guides shown here:

http://docs.opencv.org/doc/tutorials/introduction/desktop_java/java_dev_intro.html
http://www.exintopro.net/blog/2013/10/02/setting-up-eclipse-for-using-opencv-java-in-ubuntu/

