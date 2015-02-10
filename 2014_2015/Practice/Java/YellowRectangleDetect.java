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

import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;

import com.atul.JavaOpenCV.Imshow;

public class YellowRectangleDetect {

	// Detects yellow totes
	public static void yellowRectangleDetect(Mat frame) {
		if (frame.empty()) {
			System.out.println("Camera not found.");
			System.exit(0);
		}
		
		Mat edges = new Mat();
		MatOfPoint2f MOP2f = new MatOfPoint2f();
		ArrayList<MatOfPoint> contour = new ArrayList<MatOfPoint>();
		
		Core.inRange(frame, new Scalar(26 , 30 , 32) , new Scalar(33 , 255 , 255), frame);
		
		Imgproc.Canny(frame, edges, 0, 100);
		Imgproc.findContours(edges, contour, new Mat() , Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
		
		// Use Iterator for java to make it work
		for (Iterator<MatOfPoint> iterator = contour.iterator(); iterator.hasNext();){
			MatOfPoint temp =  (MatOfPoint) iterator.next();
			if (temp.width() * temp.height() < 5) {
				iterator.remove();
			}
		}
		
		System.out.println(contour.size());
		for (int i = 0; i < contour.size(); i++){
			contour.get(i).convertTo(MOP2f, CvType.CV_32FC2);
			Imgproc.approxPolyDP(MOP2f, MOP2f, 2 , true);
		}
	}
	
	// Variables for track
	static Mat edges = new Mat();
	static List<MatOfPoint> MOP = new ArrayList<MatOfPoint>();
	static Rect rect = Rect(0,0,0,0);
	
	public static Mat track(Mat yellow){
		Mat finale = yellow.clone();
		
		Imgproc.Canny(finale, edges, 0, 100);
		Imgproc.findContours(edges, MOP, new Mat() , Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
		
		return finale;
	}
	
	public static void main(String[] args) {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);		
		VideoCapture cap = new VideoCapture(0);
		Mat src = new Mat();
		
		if (!cap.isOpened()) {
			System.out.println("VideoCapture device not detected.");
			System.exit(0);
		}
		
		Imshow cam = new Imshow("Feed");

		for (;;) {
			if (cap.grab()) {
				try {
					if (cap.retrieve(src)) {
						Imgproc.cvtColor(src, src, Imgproc.COLOR_BGR2HSV);
						yellowRectangleDetect(src);
						cam.showImage(src);
					}
					
				} catch(Exception e) {
					
				}
			}
		}
			}
}