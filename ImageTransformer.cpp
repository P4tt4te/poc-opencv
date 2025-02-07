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

std::vector<cv::Mat> ImageTransformer::getSplittedImg(int _width, int _height)
{
	cv::Mat filteredImage = getImg();
	cv::Mat initialImage = getInitialImg();
	cv::Size fitSize = cv::Size(_width, _height);

	cv::Mat filteredImageResized;
	resize(filteredImage, filteredImageResized, fitSize, cv::INTER_LINEAR);
	cv::Mat initialImageResized;
	resize(initialImage, initialImageResized, fitSize, cv::INTER_LINEAR);

	std::vector<cv::Mat> result;

	cv::Rect initialRect(0, 0, _width / 2, _height);
	cv::Rect filteredRect(_width / 2, 0, _width / 2, _height);

	result.push_back(initialImageResized(initialRect));
	result.push_back(filteredImageResized(filteredRect));

	return result;
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