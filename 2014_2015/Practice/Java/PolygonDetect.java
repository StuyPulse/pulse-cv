import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

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
		int polyFound = 0;
		Mat gray = new Mat();
		Mat edges = new Mat();
		List<MatOfPoint> contour = new ArrayList<MatOfPoint>();
		MatOfPoint2f MOP2f1 = null;
		MatOfPoint2f MOP2f2 = null;
		MatOfPoint contourMOP = null;
		
		Imgproc.cvtColor(src , gray , Imgproc.COLOR_BGR2GRAY);
		Imgproc.Canny(gray, edges, 0, 100);
		Imgproc.findContours(edges, contour, null, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
		for (int i = 0 ; i < contour.size() ; i++) {
			contour.get(i).convertTo(MOP2f1, CvType.CV_32FC2);
			Imgproc.approxPolyDP(MOP2f1 , MOP2f2 , 0.02 , true);
			MOP2f2.convertTo(contourMOP, CvType.CV_32S);
			if(Math.abs(Imgproc.contourArea(contour.get(i))) < 10 || Imgproc.isContourConvex(contourMOP)) {
				continue;
			}
			if (contour.size() == poly) {
				polyFound++;
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