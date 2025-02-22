#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "poc-opencv.h"


int main()
{
	// int argc, char** argv
	/*
	cv::CommandLineParser parser(argc, argv,
		"{help  h           |            | Print this message}"
		"{image1 i1         |            | Path to the input image1. Omit for detecting through VideoCapture}"
		"{image2 i2         |            | Path to the input image2. When image1 and image2 parameters given then the program try to find a face on both images and runs face recognition algorithm}"
		"{video v           | 0          | Path to the input video}"
		"{scale sc          | 1.0        | Scale factor used to resize input video frames}"
		"{fd_model fd       | face_detection_yunet_2021dec.onnx| Path to the model. Download yunet.onnx in https://github.com/opencv/opencv_zoo/tree/master/models/face_detection_yunet}"
		"{fr_model fr       | face_recognition_sface_2021dec.onnx | Path to the face recognition model. Download the model at https://github.com/opencv/opencv_zoo/tree/master/models/face_recognition_sface}"
		"{score_threshold   | 0.9        | Filter out faces of score < score_threshold}"
		"{nms_threshold     | 0.3        | Suppress bounding boxes of iou >= nms_threshold}"
		"{top_k             | 5000       | Keep top_k bounding boxes before NMS}"
		"{save s            | false      | Set true to save results. This flag is invalid when using camera}"
	);
	*/
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
