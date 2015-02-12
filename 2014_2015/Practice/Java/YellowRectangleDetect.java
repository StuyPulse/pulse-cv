import java.util.ArrayList;

import java.util.Iterator;
import java.util.List;

import org.opencv.core.Point;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;

import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;

import com.atul.JavaOpenCV.Imshow;

public class YellowRectangleDetect {

	// Detects yellow totes and draws rectangles around them. 
	public static Mat yellowRectangleDetect(Mat frame) {
		if (frame.empty()) {
			System.out.println("Camera not found.");
			System.exit(0);
		}

		Mat tracked = frame.clone();
		Mat edges = new Mat();
		Rect rect = new Rect(0,0,0,0);
		MatOfPoint2f MOP2f = new MatOfPoint2f();
		ArrayList<MatOfPoint> contour = new ArrayList<MatOfPoint>();

		Imgproc.cvtColor(frame, frame, Imgproc.COLOR_BGR2HSV);
		Core.inRange(frame, new Scalar(23 , 25 , 102) , new Scalar(33 , 255 , 255), frame);
		//Core.inRange(frame, new Scalar(0 , 197 ,206) , new Scalar(164, 255, 239), frame);

		Imgproc.Canny(frame, edges, 0, 100);
		Imgproc.findContours(edges, contour, new Mat() , Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

		// Use Iterator to go through the contours
		for (Iterator<MatOfPoint> iterator = contour.iterator(); iterator.hasNext();){
			MatOfPoint temp =  (MatOfPoint) iterator.next();
			// Use width times height instead of contourArea because java
			if (temp.width() * temp.height() < 50) {
				iterator.remove();
			}
		}

		// Go through each contour
		for (int i = 0; i < contour.size(); i++){

			contour.get(i).convertTo(MOP2f, CvType.CV_32FC2);
			Imgproc.approxPolyDP(MOP2f, MOP2f, 2 , true);
			List<Point> contourList = MOP2f.toList();

			// If the object has 4 sides, then it's a rectangle,
			// and draws a rectangle around it.
			if (contourList.size() == 4) {
				MatOfPoint pts = new MatOfPoint(MOP2f.toArray());
				rect = Imgproc.boundingRect(pts);
				Core.rectangle(tracked, new Point(rect.x,rect.y), new Point(rect.x+rect.width,rect.y+rect.height), new Scalar(30, 255, 255), 3); 
			}
		}

		return tracked; 

	}

	// Binarialize image
	public static Mat bin(Mat orig) {
		//Core.inRange(orig, new Scalar(0 , 197 ,206) , new Scalar(164, 255, 239), orig);
		Core.inRange(orig, new Scalar(26 , 50 , 50) , new Scalar(33 , 163.2 , 255), orig);

		return orig;
	}

	public static void main(String[] args) {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);		
		VideoCapture cap = new VideoCapture(0);
		Mat src = new Mat();
		Mat dst = new Mat();

		if (!cap.isOpened()) {
			System.out.println("VideoCapture device not detected.");
			System.exit(0);
		}

		Imshow cam = new Imshow("Feed");

		for (;;) {
			if (cap.grab()) {
				try {
					if (cap.retrieve(src)) {
						// Detect the yellow rectangles
						dst = yellowRectangleDetect(src);
						cam.showImage(dst);
					}

				} catch(Exception e) {

				}
			}
		}
	}
}