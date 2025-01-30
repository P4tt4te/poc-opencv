#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageTransformer.h"

class UIWindow {
private:
	std::string sWindowName;
	ImageTransformer* ptrImageTransformer;
public:
	UIWindow(std::string _sWindowName, ImageTransformer* _imageTransformer);
	void createWindow() const;
	void drawMenu(cv::Mat& _frame, std::string& _sCurrentPage) const;
	void findPath();
	void drawEditor(cv::Mat& _frame, std::string& _sCurrentPage) const;
	void showImage(const cv::Mat& _img) const;
	void createButton() const;
	void destroyWindow() const;
	bool operator==(const UIWindow& other) const;
};