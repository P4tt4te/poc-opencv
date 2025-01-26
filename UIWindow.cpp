#include "UIWindow.h"
#include "Windows.h"
#include "WinUser.h"
#include "shellapi.h"
//#pragma comment(lib, "shell32")

UIWindow::UIWindow(std::string _sWindowName)
{
	sWindowName = _sWindowName;
	
}

void UIWindow::findPath()
{
	ShellExecute(NULL, "open", "C://", NULL, NULL, SW_SHOWNORMAL);
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
