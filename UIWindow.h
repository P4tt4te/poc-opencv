#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class UIWindow {
public:
	UIWindow(std::string _sWindowName);
	void findPath();
	void showImage(const cv::Mat& _img) const;
	void destroyWindow() const;
private:
	std::string sWindowName;

};