import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.CvType;
import org.opencv.imgproc.Imgproc;
import org.opencv.core.Scalar;

public class Test {

    public static Mat getGreen(Mat original) {
        Mat processed = new Mat(256 , 256 , CvType.CV_8UC3);
        Mat HSV = new Mat();
        Imgproc.cvtColor(original , HSV , Imgproc.COLOR_BGR2HSV);
        Core.inRange(HSV , new Scalar(42d , 60d , 15d) , new Scalar(67d , 255d , 90d) , processed);
        return processed;
    }

    public static void printMat() {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        Mat test = Mat.eye(3 ,  3 , CvType.CV_8UC1);
        System.out.println("Test Mat = \n" + test.dump());
    }

}
