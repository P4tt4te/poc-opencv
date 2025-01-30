#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "ImageTransformer.h"
#include "UIWindow.h"
#include <windows.h>
#include <stdio.h>

int main()
{
	std::string sMenuInput;

	// Window properties
	std::string sCurrentPage = "menu";
	cv::Mat frame;
	
	// C:/Users/edwar/Pictures/Acer/Acer_Wallpaper_02_5000x2813.jpg
	ImageTransformer imgTrans("");

	UIWindow window("Image edition automation");
	window.createWindow();

	while (true)
	{
		frame = cv::Mat(cv::Size(600, 400), CV_8UC3);
		if (sCurrentPage == "menu")
		{
			window.drawMenu(frame);
		} else if (sCurrentPage == "editor")
		{
			window.drawEditor();
		} else
		{
			break;
		}

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
