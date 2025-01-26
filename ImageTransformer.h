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
	const cv::Mat& getImg() const { return mImg; }
	void blur(const int _size) const;
	void medianBlur(const int _size) const;
	void gaussianBlur(const int _size) const;
};
