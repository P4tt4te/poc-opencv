#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class UIWindow {
public:
	UIWindow(std::string _sWindowName);
	void createWindow() const;
	void drawMenu(cv::Mat& _frame) const;
	void findPath();
	void drawEditor() const;
	void showImage(const cv::Mat& _img) const;
	void createButton() const;
	void destroyWindow() const;
private:
	std::string sWindowName;

};