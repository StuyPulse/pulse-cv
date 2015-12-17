import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.highgui.VideoCapture;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;

import com.atul.JavaOpenCV.Imshow;

public class CameraTest {

    public static void binarilizeImage(Mat src , Mat bin_image , Imshow BIN_IMAGE_SHOW) {
        Mat grey = new Mat();
        Imgproc.cvtColor(src , grey , Imgproc.COLOR_BGR2GRAY);
        Imgproc.threshold(grey , bin_image , 128 , 255 , Imgproc.THRESH_BINARY);
        BIN_IMAGE_SHOW.showImage(bin_image);
    }

    public static void grayImage(Mat src , Mat grey_image , Imshow GREY_IMAGE_SHOW) {
        Imgproc.cvtColor(src , grey_image , Imgproc.COLOR_BGR2GRAY);
        GREY_IMAGE_SHOW.showImage(grey_image);
    }

    public static void main(String[] args) {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);

        Mat test1 = Highgui.imread("/Users/photoXin/Development/StuyPulse/pulse-cv/2014_2015/Practice/QuadrilateralDetect/QUAD2.png");
        Mat test2 = Highgui.imread("/Users/photoXin/Development/StuyPulse/pulse-cv/2014_2015/Practice/QuadrilateralDetect/QUAD3.png");

        System.out.println(GreenDetect.polygonDetect(test1 , 4 , false));
        System.out.println(GreenDetect.polygonDetect(test2 , 4 , false));

        Mat bin_image = new Mat();
        Mat grey_image = new Mat();
        Imshow BIN_IMAGE_SHOW = new Imshow("Binary Image");
        Imshow GREY_IMAGE_SHOW = new Imshow("Greyscale Image");
        Imshow DEF_IMAGE_SHOW = new Imshow("Unprocessed Image");

        VideoCapture cap = new VideoCapture(0);

        if (!cap.isOpened()) {
            System.out.println("VideoCapture device not detected.");
            System.exit(0);
        }
        Mat src = new Mat();
        while (true) {

            if (cap.grab()) {
                try {
                    if (cap.retrieve(src)) {
                        DEF_IMAGE_SHOW.showImage(src);
                        binarilizeImage(src , bin_image , BIN_IMAGE_SHOW);
                        grayImage(src , grey_image , GREY_IMAGE_SHOW);
                        //BIN_IMAGE_SHOW.showImage(Test.getGreen(src));
                    }
                } catch(Exception e) {

                }
            }
        }

    }

}
