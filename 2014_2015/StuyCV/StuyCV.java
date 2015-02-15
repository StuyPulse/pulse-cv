import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;

import com.atul.JavaOpenCV.Imshow;

public class StuyCV {

	/***
	 * Finds the area of a rectangle
	 * @param rect - Rectangle that will be processed
	 * @return - Area of the rectangle
	 */
	public static double contourArea(Rect rect) {
		return rect.width * rect.height;
	}

	/***
	 * Finds the area of a MatOfPoint
	 * @param mop - MatOfPoint to be processed
	 * @return - Area of the MatOfPoint
	 */
	public static double contourArea(MatOfPoint mop) { 
		return mop.width() * mop.height();
	}

	/***
	 * Gets feed from the camera
	 * @param camera - VideoCapture that will be used
	 * @param src - Source matrix that will be shown
	 */
	public static void getFromFeed(VideoCapture camera, Mat src) {   

		// If the camera is not found, then program will exit
		if (!camera.isOpened()) {
			System.out.println("VideoCapture device not detected.");
			throw new NullPointerException();
		}

		if (camera.grab()) {
			if (!camera.retrieve(src)) {
				System.out.println("Could not retrieve.");
				throw new NullPointerException();
			}
		}
	}

	/***
	 * Reads a picture with a given path
	 * @param path - Absolute path to the picture
	 * @return - The picture
	 */
	public static Mat readPicture(String path) {
		return Highgui.imread(path);
	}

	/**
	 * Shows a picture with a given path
	 * @param path - Absolute path to the picture
	 */
	public static void showPicture(String path) {
		Imshow img = new Imshow("Picture");
		Mat pic = new Mat();
		pic = readPicture(path);
		img.showImage(pic);
	}

	/***
	 * Detects color
	 * @param src - Source matrix to be processed
	 * @param lowerBound - Lower scalar of colors for the range
	 * @param upperBound - Upper scalar of colors for the range
	 * @return - Processed matrix
	 */
	public static Mat colorDetect(Mat src , Scalar lowerBound , Scalar upperBound) { 

		Imgproc.cvtColor(src, src, Imgproc.COLOR_BGR2HSV);

		// Use lower and upper bounds as declared above
		Core.inRange(src, lowerBound, upperBound, src);

		// Sequence of erode and dilate, and finally a threshold to reduce noise
		Imgproc.erode(src , src , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));
		Imgproc.dilate(src , src , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));
		Imgproc.dilate(src , src , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));
		Imgproc.erode(src , src , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));

		Imgproc.threshold(src, src, 127, 255, Imgproc.THRESH_BINARY);

		return src;
	}


	/***
	 * Detects the edges in a matrix
	 * @param src - Matrix to be processed
	 * @return - Processed List of MatOfPoint
	 */
	public static ArrayList<MatOfPoint> edgeDetect(Mat src){ 

		// Destination matrix for the processed image
		Mat edges = new Mat();
		// Returned ArrayList<MatOfPoint> for return
		ArrayList<MatOfPoint> contour = new ArrayList<MatOfPoint>();

		Imgproc.Canny(src, edges, 0, 100);
		// New Mat as the hierarchy that is being superimposed
		// If the hierarchy is the source, then the destination will return all white
		Imgproc.findContours(edges, contour, new Mat() , Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

		return contour;
	}


	/***
	 *  Function takes an ArrayList<MatOfPoint> and a double to compare the area of the  
	 *  ArrayList<MatOfPoint> and the inputed area
	 * @param src - List of MatOfPoints to be processed
	 * @param area - The lower threshold of valid areas. Use 0 for no threshold 
	 * @return - Processed matrix
	 */
	public static ArrayList<MatOfPoint> iterator (ArrayList<MatOfPoint> src, double area) {  

		// As long as the next element in src is not empty
		for (Iterator<MatOfPoint> iterator = src.iterator(); iterator.hasNext();){
			MatOfPoint temp =  (MatOfPoint) iterator.next();

			if (contourArea(temp) < area) {
				iterator.remove();
			}
		}

		return src;
	}


	/***
	 * Detects number of polygons. 
	 * @param src - Mat to be processed
	 * @param side - Number of edges in the desired polygon
	 * @param epsilon - How accurate the polygon detection will be (Should be small)
	 * @return - Number of polygons with edges sides
	 */
	public static int polygonDetect(Mat src, int sides, int epsilon) { 

		MatOfPoint2f MOP2f = new MatOfPoint2f();
		List<MatOfPoint> contour = new ArrayList<MatOfPoint>();
		MatOfPoint contourMOP = new MatOfPoint();
		int polygonsFound = 0;
		Mat gray = new Mat();
		Mat edges = new Mat();

		Imgproc.cvtColor(src, gray, Imgproc.COLOR_BGR2GRAY);
		
		contour = edgeDetect(gray);

		for (int i = 0; i < contour.size(); i++) {

			contour.get(i).convertTo(MOP2f, CvType.CV_32FC1);
			Imgproc.approxPolyDP(MOP2f, MOP2f, epsilon, true);
			MOP2f.convertTo(contourMOP, CvType.CV_32SC2);
			List<Point> contourList = contourMOP.toList();

			if (contourList.size() == sides) {
				polygonsFound++;
			}
		}

		return polygonsFound;
	}


	/***
	 * Detects motion with hard coded threshold 
	 * @param stream - Input VideoCapture that will be processed
	 * @param timeout - The length of time in milliseconds to check for motion
	 * @return True if motion is detected, false otherwise
	 */
	public static boolean motionDetect(VideoCapture stream , double timeout) { 

		Mat prev = new Mat();
		Mat src = new Mat();
		Mat compare = new Mat();
		double totalArea;

		double startTime = System.currentTimeMillis();

		getFromFeed(stream , src);

		while (System.currentTimeMillis() - startTime < timeout) {
			prev = src.clone();

			Core.bitwise_and(prev, src, compare);
			ArrayList<MatOfPoint> contours = new ArrayList<MatOfPoint>();
			contours = edgeDetect(compare);
			totalArea = 0.0;

			for (MatOfPoint mop : contours) {
				totalArea += contourArea(mop);
			}

			if (totalArea >= 15) {
				return true;
			}
		}
		return false;
	}

	/***
	 * Detects motion using a threshold
	 * @param stream - Input VideoCapture that will be processed
	 * @param timeout - The length of time in milliseconds to check for motion 
	 * @param threshold - The tolerance of motion for detection
	 * @return True if motion is detected, false otherwise
	 */
	public static boolean motionDetect(VideoCapture stream , double timeout, int threshold) { 

		Mat prev = new Mat();
		Mat src = new Mat();
		Mat compare = new Mat();
		double totalArea;

		double startTime = System.currentTimeMillis();

		getFromFeed(stream , src);

		while (System.currentTimeMillis() - startTime < timeout) {
			prev = src.clone();

			Core.bitwise_and(prev, src, compare);
			ArrayList<MatOfPoint> contours = new ArrayList<MatOfPoint>();
			contours = edgeDetect(compare);
			totalArea = 0.0;

			for (MatOfPoint mop : contours) {
				totalArea += contourArea(mop);
			}

			if (totalArea >= threshold) {
				return true;
			}
		}
		return false;
	}
}