import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Size;
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
        Rect firstRect = new Rect(0,0,0,0);
        Rect secondRect = new Rect(0,0,0,0);
        MatOfPoint2f MOP2f = new MatOfPoint2f();
        ArrayList<MatOfPoint> contour = new ArrayList<MatOfPoint>();

        // Convert to HSV so that we can detect the range
        Imgproc.cvtColor(frame, frame, Imgproc.COLOR_BGR2HSV);

        // Working yellow ranges
        Core.inRange(frame, new Scalar(15 , 100 , 100) , new Scalar(35 , 255 , 255), frame);

        // Smooth edges and find contours
        Imgproc.Canny(frame, edges, 0, 100);
        Imgproc.GaussianBlur(frame, edges, new Size(3,3), 1.5, 1.5);

        // Sequence of erode and dilate to remove weird noise
        Imgproc.erode(edges , edges , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));
        Imgproc.dilate(edges , edges , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));
        Imgproc.dilate(edges , edges , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));
        Imgproc.erode(edges , edges , Imgproc.getStructuringElement(Imgproc.MORPH_RECT , new Size(3,3)));

        Imgproc.threshold(edges, edges, 127, 255, Imgproc.THRESH_BINARY);
        Imgproc.findContours(edges, contour, new Mat() , Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        double currLargestArea = -1.0;
        double currSecondArea = -2.0;
        int max_polygon_index = -1; // The index of the largest polygon
        int second_polygon_index = -2;

        List<Point> contourList = null;

        // Getting the raw contours
        // Convert to MatOfPoint2f to use approxPolyDP
        for (int i = 0; i < contour.size(); i++) {

            contour.get(i).convertTo(MOP2f, CvType.CV_32FC1);
            Imgproc.approxPolyDP(MOP2f, MOP2f, 2 , true);
            // Convert into a list
            contourList = MOP2f.toList();
        }

        double area = -1.0;

        /* for (int i = 1 ; i < contour.size() ; i++) {

           if (contourList.size() == 4) {
           area = contour.get(i).width() * contour.get(i).height();

           if (area >= currLargestArea) {
           max_polygon_index = i;
           currLargestArea = area;
           } else if (area < currLargestArea && area >= currSecondArea) {
           System.out.println(area < currLargestArea);
           System.out.println(area);
           System.out.println(area > currSecondArea);
           second_polygon_index = i;
           currSecondArea = area;
           }
           }
           } */

        Collections.sort(contour, new Comparator<MatOfPoint>() {
            public int compare(MatOfPoint first, MatOfPoint second) {

                double firstArea = first.width() * first.height();
                double secondArea = second.width() * second.height();

                double diff = secondArea - firstArea;

                return (int) diff;

            }
        }
        );

        if(contour.size() >= 2) {
            contour.add(contour.remove(0));
            MatOfPoint pts = new MatOfPoint(contour.get(0));
            MatOfPoint pts2 = new MatOfPoint(contour.get(1));

            firstRect = Imgproc.boundingRect(pts);
            secondRect = Imgproc.boundingRect(pts2);

            // We know that the origin (0,0) is at the top left
            Point firstTopLeft = new Point(firstRect.x, firstRect.y);
            Point secondTopLeft = new Point(secondRect.x, secondRect.y);

            // y coordinate has to be rect.y + rect.height because subtracting brings the boundingRect up?
            Point firstLowerRight = new Point(firstRect.x+firstRect.width, firstRect.y+firstRect.height);
            Point secondLowerRight = new Point(secondRect.x+secondRect.width, secondRect.y+secondRect.height);

            Core.rectangle(tracked, firstTopLeft, firstLowerRight, new Scalar(0, 255, 0), 2);
            Core.rectangle(tracked, secondTopLeft, secondLowerRight, new Scalar(255, 0, 0), 2);

        }

        return tracked;
    }

    // Binarialize the image for debugging color values
    public static Mat bin(Mat orig) {
        Imgproc.cvtColor(orig, orig, Imgproc.COLOR_BGR2HSV);
        Core.inRange(orig, new Scalar(23 , 50 , 50) , new Scalar(35 , 255 , 255), orig);

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
        Imshow cam2 = new Imshow("Binary");

        // Live feed

        long starttime = System.currentTimeMillis();
        for (;;) {
            if (cap.grab() && System.currentTimeMillis() - starttime > 150) {
                starttime = System.currentTimeMillis();
                try {
                    if (cap.retrieve(src)) {
                        dst = src.clone();
                        dst = bin(dst);
                        src = yellowRectangleDetect(src);
                        cam.showImage(src);
                        cam2.showImage(dst);
                    }

                } catch(Exception e) {

                }
            }
        }

        // Single image test (note that single image works better than live feed)
        /*
           Mat img = Highgui.imread("/home/james/Dev/pulse-cv/2014_2015/Practice/Java/YELLOW.png");
           img = yellowRectangleDetect(img);
           cam.showImage(img); */

    }
}
