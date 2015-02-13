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
	
		// Convert to HSV so that we can detect the range
		Imgproc.cvtColor(frame, frame, Imgproc.COLOR_BGR2HSV);
		
		// Work in progress yellow ranges
		Core.inRange(frame, new Scalar(22 , 50 , 50) , new Scalar(29.5 , 255 , 255), frame);
				
		// Smooth edges and find contours
		Imgproc.Canny(frame, edges, 0, 100);
		Imgproc.findContours(edges, contour, new Mat() , Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
	
		// Use Iterator to go through the contours
		for (Iterator<MatOfPoint> iterator = contour.iterator(); iterator.hasNext();){
			MatOfPoint temp =  (MatOfPoint) iterator.next();
			
			// Use width times height instead of contourArea because java 
			// Width * height is different in live feed and camera
			if (temp.width() * temp.height() < 50) {
				iterator.remove();
				
			}
		}
	
		// Go through each contour
		for (int i = 0; i < contour.size(); i++){
			
			// Convert to MatOfPoint2f to use approxPolyDP
			contour.get(i).convertTo(MOP2f, CvType.CV_32FC2);
			Imgproc.approxPolyDP(MOP2f, MOP2f, 2 , true);
			// Convert into a list
			List<Point> contourList = MOP2f.toList();
			
			// If the object has 4 sides, then it's a rectangle,
			// and draws a rectangle around it. 
			if (contourList.size() == 4) {
				MatOfPoint pts = new MatOfPoint(MOP2f.toArray());
				rect = Imgproc.boundingRect(pts);
				
				// We know that the origin (0,0) is at the top left
				Point topLeft = new Point(rect.x, rect.y);
				Point lowerRight = new Point(rect.x+rect.width, rect.y-rect.height);
				
				Core.rectangle(tracked, topLeft, lowerRight, new Scalar(30, 100, 255), 2);
			}
		}
	
		return tracked; 
	
	}
	
	// Binarialize the image
	public static Mat bin(Mat orig) {
		Imgproc.cvtColor(orig, orig, Imgproc.COLOR_BGR2HSV);
		Core.inRange(orig, new Scalar(24 , 50 , 50) , new Scalar(38 , 255 , 255), orig);
	
		return orig;
		
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
	
		// Live feed
		/*
		for (;;) {
			if (cap.grab()) {
				try {
					if (cap.retrieve(src)) {
						src = yellowRectangleDetect(src);
						cam.showImage(src);
					}
	
				} catch(Exception e) {
	
				}
			}
		} */
		
		// Single image test (note that single image works better than live feed)
		
		Mat img = Highgui.imread("/home/james/Dev/pulse-cv/2014_2015/Practice/Java/YELLOW.png");
		img = yellowRectangleDetect(img);
		cam.showImage(img);
		
			}
	}