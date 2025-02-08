#include "ImageTransformer.h"

ImageTransformer::ImageTransformer(std::string _sSource) {
	sInitialSource = _sSource;
	std::replace(sInitialSource.begin(), sInitialSource.end(), '\\', '/');
	mImg = cv::imread(sInitialSource, cv::IMREAD_COLOR);
	mInitialImg = mImg;
}

void ImageTransformer::setSource(std::string _sSource) {
	sInitialSource = _sSource;
	std::replace(sInitialSource.begin(), sInitialSource.end(), '\\', '/');
	mInitialImg = cv::imread(sInitialSource);
	mImg = mInitialImg;
}

void ImageTransformer::getSplittedImg(std::vector<cv::Mat>& _vecImages,int _width, int _height, double _percentage)
{
	cv::Mat filteredImage = getImg();
	cv::Mat initialImage = getInitialImg();
	cv::Size fitSize = cv::Size(_width, _height);

	cv::Mat filteredImageResized;
	resize(filteredImage, filteredImageResized, fitSize, cv::INTER_LINEAR);
	cv::Mat initialImageResized;
	resize(initialImage, initialImageResized, fitSize, cv::INTER_LINEAR);

	int iCalculatedWidth = _width * (_percentage / 100);
	int iRestCalculatedWidth = _width - iCalculatedWidth;
	cv::Rect initialRect(0, 0, iCalculatedWidth, _height);
	cv::Rect filteredRect(iCalculatedWidth, 0, iRestCalculatedWidth, _height);

	_vecImages.push_back(initialImageResized(initialRect));
	_vecImages.push_back(filteredImageResized(filteredRect));
}

void ImageTransformer::blur(int _size) {
	mInitialImg = cv::imread(sInitialSource);
	mImg = mInitialImg;
	cv::Mat imgBlur = mInitialImg;
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