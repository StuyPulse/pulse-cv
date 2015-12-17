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

import com.atul.JavaOpenCV.Imshow;

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
        for (int i = 0 ; i < contour.size() ; i++) {
            contour.get(i).convertTo(MOP2f1, CvType.CV_32FC2);
            Imgproc.approxPolyDP(MOP2f1 , MOP2f1, 1 , true); //Changed epsilon from 0.02 to 1 (Quad detection works).
            MOP2f1.convertTo(contourMOP, CvType.CV_32SC2);
            if(Math.abs(Imgproc.contourArea(contour.get(i))) < 10 || Imgproc.isContourConvex(contourMOP)) {
                //break;
            }
            List<Point> contourList = contourMOP.toList();
            if (contourList.size() == poly) {
                polyFound++;
            }
        }
        System.out.println("Polygons detected: " + polyFound);
        Imshow im = new Imshow("Preview");
        im.showImage(gray); //Show image for reference
        return polyFound;
    }

    public static void main(String[] args) {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        Mat img = Highgui.imread("/home/james/Dev/pulse-cv/2014_2015/Practice/QuadrilateralDetect/QUAD5.png");
        polyDetect(img , 4);
    }
}
