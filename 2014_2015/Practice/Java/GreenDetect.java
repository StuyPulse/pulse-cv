import java.util.ArrayList;

import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.imgproc.Imgproc;

public class GreenDetect {

    public static int polygonDetect(Mat src , int verts , boolean isGray) {
        if (src.empty()) {
            System.out.println("Source image for polygon detect empty. Returning -1");
            return -1;
        }
        int numDetected = 0;
        Mat gray = new Mat();
        if (isGray) {
            Imgproc.cvtColor(src , gray , Imgproc.COLOR_BGR2GRAY);
        } else {
            gray = src;
        }
        Mat edges = new Mat();
        Imgproc.Canny(gray, edges, 0 , 100);
        ArrayList<MatOfPoint> contours = new ArrayList<MatOfPoint>();
        Imgproc.findContours(edges , contours, edges, Imgproc.RETR_EXTERNAL , Imgproc.CHAIN_APPROX_SIMPLE);
        MatOfPoint2f approx = new MatOfPoint2f();
        MatOfPoint2f CONTS = new MatOfPoint2f();
        for (int i = 0 ; i < contours.size() ; i++) {
            CONTS = new MatOfPoint2f(contours.get(i).toArray());
            Imgproc.approxPolyDP(CONTS, approx, Imgproc.arcLength(CONTS , true) * 0.02 , true);
        }
        return numDetected;
    }

}
