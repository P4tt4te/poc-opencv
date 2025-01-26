#include "UIWindow.h"

UIWindow::UIWindow(std::string _sWindowName)
{
	sWindowName = _sWindowName;
}

void UIWindow::showImage(const cv::Mat& _img) const
{
	cv::imshow(sWindowName, _img);
	cv::waitKey(0);
}

void UIWindow::destroyWindow() const
{
	cv::destroyWindow(sWindowName);
}
