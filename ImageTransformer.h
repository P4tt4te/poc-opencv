#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class ImageTransformer {
private:
	std::string sInitialSource;
	cv::Mat mInitialImg;
	cv::Mat mImg;
public:
	ImageTransformer(std::string _sSource);
	void setSource(std::string _sSource);
	cv::Mat& getImg() { return mImg; };
	cv::Mat getInitialImg() const { return cv::imread(sInitialSource); };
	std::vector<cv::Mat> getSplittedImg(int _width, int _height);
	// Image processing
	void blur(int _size);
	void medianBlur(int _size) const;
	void gaussianBlur(int _size) const;
};
