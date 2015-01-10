import java.util.ArrayList;
import java.util.List;

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.imgproc.Imgproc;

public class PolygonDetect {

	public static int polyDetect(Mat src , int poly) {
		if(src.empty()) {
			System.out.println("Image not found.");
			return -1;
		}
		Mat gray = new Mat();
		int polyFound = 0;
		Imgproc.cvtColor(src , gray , Imgproc.COLOR_BGR2GRAY);
		Mat edges = new Mat();
		Imgproc.Canny(gray, edges, 0, 100);
		List<MatOfPoint> contour = new ArrayList<>();
		Imgproc.findContours(edges, contour, null, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
		for (int i = 0 ; i < contour.size() ; i++) {
			// Have to fix approxpolydp function & finish 'if' statement found below
			MatOfPoint2f mat = new MatOfPoint2f((Point[]) contour.toArray());
			Imgproc.approxPolyDP([i] , mat , 0.02 , true);
			if(Math.abs(contourArea(contour[i]) < 10 || !isContourConvex(contour))) {
				
			}
		}
		return polyFound;
	}
	
	public static void main(String[] args) {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		Mat img = new Mat();
		polyDetect(img , 4);
	}
}