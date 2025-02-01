#include <windows.h>
#include <stdio.h>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "UIWindow.h"


int main()
{
	std::string sMenuInput;

	// Window properties
	std::string sCurrentPage = "menu";
	cv::Mat frame;
	
	// C:/Users/edwar/Pictures/Acer/Acer_Wallpaper_02_5000x2813.jpg
	std::string sFullPathName = "";
	char fullFilename[MAX_PATH];
	GetFullPathName("./assets/placeholder.jpg", MAX_PATH, fullFilename, nullptr);
	sFullPathName = fullFilename;

	ImageTransformer imageTransformInstance(sFullPathName);
	ImageTransformer* ptrImageInstance = &imageTransformInstance;

	UIWindow window("Image edition automation", ptrImageInstance);
	window.createWindow();

	while (true)
	{
		frame = cv::Mat(cv::Size(600, 400), CV_8UC3);
		if (sCurrentPage == "menu")
			window.drawMenu(frame, sCurrentPage);
		else if (sCurrentPage == "editor")
			window.drawEditor(frame, sCurrentPage);
		else
			break;

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}

	/*
	std::transform(sMenuInput.begin(), sMenuInput.end(), sMenuInput.begin(),
			[](unsigned char c) { return std::toupper(c); }
		);
	*/

	return 0;
}
