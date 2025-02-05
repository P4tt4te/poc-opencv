#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageTransformer.h"
#include "Helpers.h"

class UIWindow {
private:
	std::string sWindowName;
	ImageTransformer* ptrImageTransformer;
	// Editor properties
	int iEditorWidth = 150;
	int iEditorHeight = 200;
	// Editor values
	double dBlurValue = 0.0;
	// Drag properties
	bool bDragging = false;
	int iEditorX = 10;
	int iEditorY = 10;
	int iCursorX = 0;
	int iCursorY = 0;
	int iCursorXOld = 0;
	int iCursorYOld = 0;
public:
	UIWindow(std::string _sWindowName, ImageTransformer* _imageTransformer);
	void createWindow() const;
	void drawMenu(cv::Mat& _frame, std::string& _sCurrentPage) const;
	void findPath();
	void drawEditor(cv::Mat& _frame, std::string& _sCurrentPage);
	void createButton() const;
	void destroyWindow() const;
};