#include "ImageTransformer.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

ImageTransformer::ImageTransformer(std::string _sSource) {
	sInitialSource = _sSource;
}

void ImageTransformer::blur() const {
	cv::Mat img = cv::imread(sInitialSource);
	cv::Mat imgBlur;
	cv::blur(img, imgBlur, cv::Size(5, 5));
	cv::imshow("Blur", imgBlur);
	cv::waitKey(0);
}