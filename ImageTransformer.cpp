#include "ImageTransformer.h"

ImageTransformer::ImageTransformer(std::string _sSource) {
	sInitialSource = _sSource;
	mImg = cv::imread(sInitialSource, cv::IMREAD_COLOR);
}

void ImageTransformer::setSource(std::string _sSource) {
	sInitialSource = _sSource;
	mInitialImg = cv::imread(sInitialSource);
	mImg = mInitialImg;
}

void ImageTransformer::blur(int _size) const {
	cv::Mat imgBlur = mImg;
	cv::blur(imgBlur, mImg, cv::Size(_size, _size));
}

void ImageTransformer::medianBlur(int _size) const {
	cv::Mat imgBlur = mImg;
	cv::medianBlur(imgBlur, mImg, _size);
}

void ImageTransformer::gaussianBlur(int _size) const {
	cv::Mat imgBlur = mImg;
	cv::GaussianBlur(imgBlur, mImg, cv::Size(_size, _size), 0);
}