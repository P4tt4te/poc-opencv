#include "ImageTransformer.h"

ImageTransformer::ImageTransformer(std::string _sSource) {
	sInitialSource = _sSource;
	mImg = cv::imread(sInitialSource);
}

void ImageTransformer::setSource(std::string _sSource) {
	sInitialSource = _sSource;
	mInitialImg = cv::imread(sInitialSource);
	mImg = mInitialImg;
}

void ImageTransformer::blur(const int _size) const {
	cv::Mat imgBlur = mImg;
	cv::blur(imgBlur, mImg, cv::Size(_size, _size));
}

void ImageTransformer::medianBlur(const int _size) const {
	cv::Mat imgBlur;
	cv::medianBlur(mImg, imgBlur, _size);
	cv::imshow("Median Blur", imgBlur);
	cv::waitKey(0);
}

void ImageTransformer::gaussianBlur(const int _size) const {
	cv::Mat imgBlur;
	cv::GaussianBlur(mImg, imgBlur, cv::Size(_size, _size), 0);
	///mImg = imgBlur;
}