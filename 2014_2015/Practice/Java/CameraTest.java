import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;

import com.atul.JavaOpenCV.Imshow;

@SuppressWarnings("serial")
public class CameraTest extends JFrame implements KeyListener{

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
					}
				} catch(Exception e) {
					
				}
			}
		}
	}

	@Override
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
		if (arg0.getID() == KeyEvent.VK_ESCAPE) {
			System.exit(0);
		}
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	
}
