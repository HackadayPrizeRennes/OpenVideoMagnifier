#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <array>

cv::Mat invert_filter(const cv::Mat& src)
{
	return cv::Scalar::all(255) - src;
}

cv::Mat zoom_filter(const cv::Mat& src, const unsigned int zoom)
{
	assert(zoom < 70);
	unsigned int value = std::log(zoom)/std::log(2);
	cv::Mat res = src;
	for(unsigned int i = 0; i < value; ++i)
	{
		cv::pyrUp(res, res, cv::Size(res.cols*2, res.rows*2));
		res = res(cv::Rect(src.size().width/2,src.size().height/2,src.size().width, src.size().height));
	}
	return res;
}

cv::Mat balance_filter(const cv::Mat& src, const std::array<float,3>& percent)
{
    assert(src.channels() == 3);
    
    cv::Mat out;
 
    std::vector<cv::Mat> tmpsplit;
    split(src,tmpsplit);
    for(int i=0;i<3;i++) {
				float half_percent = percent[i] / 200.0f;
        //find the low and high precentile values (based on the input percentile)
        cv::Mat flat;
        tmpsplit[i].reshape(1,1).copyTo(flat);
        cv::sort(flat,flat,CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
        int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
        int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
        
        //saturate below the low percentile and above the high percentile
        tmpsplit[i].setTo(lowval,tmpsplit[i] < lowval);
        tmpsplit[i].setTo(highval,tmpsplit[i] > highval);
        
        //scale the channel
        normalize(tmpsplit[i],tmpsplit[i],0,255,cv::NORM_MINMAX);
    }
    merge(tmpsplit,out);
    return out;
}

int main(int argc, char* argv[])
{
	cv::VideoCapture cap(0);

	if (!cap.isOpened())
	{
		 std::cout << "No cam" << std::endl;
		 return -1;
	}

	while(1)
	{
		cv::Mat frame;

		bool bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			std::cout << "Cannot read the frame from camera" << std::endl;
			break;
		}

		cv::Mat invert = invert_filter(frame);
		cv::Mat zoom = zoom_filter(frame,2);
		std::array<float, 3> percent{ {1, 1, 1} };
		cv::Mat balanced = balance_filter(frame, percent);
		
		cv::imshow("Camera", frame);
		cv::imshow("Zoom", zoom);
		cv::imshow("Invert", invert);
		cv::imshow("Balanced", balanced);

		if(cv::waitKey(30) == 27)
		{
				std::cout << "Quit." << std::endl; 
				break; 
		}
	}

	return 0;

}
