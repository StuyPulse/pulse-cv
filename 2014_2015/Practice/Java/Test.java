import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.CvType;


public class Test {

	public static void printMat() {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		Mat test = Mat.eye(3 ,  3 , CvType.CV_8UC1);
		System.out.println("Test Mat = \n" + test.dump());
	}
	
}
