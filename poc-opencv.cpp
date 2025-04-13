#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "poc-opencv.h"


int main()
{
	std::string sCurrentPage = "menu";	
	std::string sFullPathPlaceholder = getFullPath("./assets/placeholder.jpg");

	ImageTransformer imageTransformInstance(sFullPathPlaceholder);
	ImageTransformer* ptrImageInstance = &imageTransformInstance;

	UIWindow window("Image edition automation", ptrImageInstance);
	window.createWindow();

	_SetCvWindowIcon("Image edition automation");

	while (true)
	{
		if (sCurrentPage == "menu")
			window.drawMenu(sCurrentPage);
		else if (sCurrentPage == "editor")
			window.drawEditor(sCurrentPage);
		else
			break;

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}
