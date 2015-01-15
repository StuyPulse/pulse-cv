import java.util.ArrayList;
import java.util.List;

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.highgui.Highgui;
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
		MatOfPoint2f MOP2f1 = new MatOfPoint2f();
		MatOfPoint contourMOP = new MatOfPoint();
		
		Imgproc.cvtColor(src , gray , Imgproc.COLOR_BGR2GRAY);
		Imgproc.Canny(gray, edges, 0, 100);
		Imgproc.findContours(edges, contour, edges, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
		System.out.println("Size of Contour: " + contour.size());
		for (int i = 0 ; i < contour.size() ; i++) {
			System.out.println("Checkpoint 1");
			contour.get(i).convertTo(MOP2f1, CvType.CV_32FC2);
			System.out.println("MOP2f1 = " + MOP2f1);
			System.out.println("Checkpoint 2");
			Imgproc.approxPolyDP(MOP2f1 , MOP2f1, 0.02 , true);
			System.out.println("Checkpoint 3");
			MOP2f1.convertTo(contourMOP, CvType.CV_32SC2);
			System.out.println("Checkpoint 4");
			System.out.println("Size of contourMOP: " + contourMOP.size());
			if(Math.abs(Imgproc.contourArea(contour.get(i))) < 10 || Imgproc.isContourConvex(contourMOP)) {
				System.out.println("Checkpoint 5");
				//continue;
			}
			System.out.println("Checkpoint 6");
			List<Point> contourList = contourMOP.toList();
			System.out.println("Checkpoint 7");
			System.out.println("Size of contourList: " + contourList.size());
			if (contourList.size() == poly) {
				System.out.println("Checkpoint 7.5");
				polyFound++;
			}
			System.out.println("Checkpoint 8");
		}
		System.out.println("Polygons detected: " + polyFound);
		return polyFound;
	}
	
	public static void main(String[] args) {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		Mat img = Highgui.imread("/home/sublimau5/eclipse/workspaces/CV-JAVA/src/QUAD2.png");
		polyDetect(img , 4);
	}
}