#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageTransformer.h"
#include "Helpers.h"

class UIWindow {
private:
	std::string sWindowName;
	ImageTransformer* ptrImageTransformer;
	cv::Mat mFrame;
	// Window properties
	int iWindowWidth = 800;
	int iWindowHeight = 600;
	// Editor properties
	int iEditorWidth = 215;
	int iEditorHeight = 370;
	double dImagePercentage = 50.0;
	bool bAspectRatio = false;
	bool* bFixedSize = new bool(false);
	// Editor values
	double dBlurValue = 0.0;
	int iMedianBlurValue = 3;
	double dGaussianBlurValue = 0.0;
	int iErodeValue = 3;
	bool bFaceDetection = false;
	bool bObjectDetection = false;
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
	int getWidth() const { return iWindowWidth; };
	int getHeight() const { return iWindowHeight; };
	void createWindow();
	void cleanFrame();
	void resizeFrame();
	void drawMenu(std::string& _sCurrentPage);
	bool findPath();
	void drawEditor(std::string& _sCurrentPage);
	void createButton() const;
	void destroyWindow() const;
};