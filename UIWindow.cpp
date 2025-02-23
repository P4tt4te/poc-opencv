#include "UIWindow.h"
#include "Windows.h"
#include <shobjidl.h>
#include "WinUser.h"
#include "shellapi.h"
#include "shlobj_core.h"
#include <opencv2/highgui.hpp>
#include "cvui.h"
#include <codecvt>

UIWindow::UIWindow(std::string _sWindowName, ImageTransformer* _imageTransformer)
{
	sWindowName = _sWindowName;
	ptrImageTransformer = _imageTransformer;
}

void UIWindow::createWindow()
{
	cvui::init(sWindowName);
	mFrame = cv::Mat(cv::Size(iWindowWidth, iWindowHeight), CV_8UC3);
}

void UIWindow::cleanFrame()
{
	mFrame = cv::Scalar(49, 52, 49);
}

void UIWindow::resizeFrame()
{
	mFrame = cv::Mat(cv::Size(iWindowWidth, iWindowHeight), CV_8UC3);
}

void UIWindow::drawMenu(std::string& _sCurrentPage)
{
	cleanFrame();
	bool bTrackbarHasMoved = false;
	cvui::beginColumn(mFrame, 10, 20, -1, -1, 10);
		cvui::text("Options");
		cvui::checkbox("Force Aspect Ratio", &bAspectRatio);
		cvui::checkbox("Fixed Window", bFixedSize);

		// In case of fixed window
		if (*bFixedSize)
		{
			cvui::text("Width");
			if (cvui::trackbar(200, &iWindowWidth, 800, 1920)) {
				bTrackbarHasMoved = true;
			}
				

			cvui::text("Height");
			if (cvui::trackbar(200, &iWindowHeight, 600, 1080)) {
				bTrackbarHasMoved = true;
			}			
		}


		if (cvui::button(100, 40, "Open editor")) {
			_sCurrentPage = "editor";
		}

	cvui::endColumn();
	cvui::imshow(sWindowName, mFrame);

	if (bTrackbarHasMoved)
		resizeFrame();
}

bool UIWindow::findPath()
{
	bool bResult = false;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (!SUCCEEDED(hr)) {
        fmt::report_error("Error: CoInitializeEx failed");
		return false;
    }

    IFileDialog* pFileDialog = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

    if (!SUCCEEDED(hr)) {
        fmt::report_error("Error: CoCreateInstance failed");
		return false;
    }

    // Filter configuration
    COMDLG_FILTERSPEC fileTypes[] = {
        { L"Images (*.jpg; *.jpeg; *.png)", L"*.jpg;*.jpeg;*.png" },
        { L"JPEG Files (*.jpg; *.jpeg)", L"*.jpg;*.jpeg" },
        { L"PNG Files (*.png)", L"*.png" },
        { L"All files (*.*)", L"*.*" }
    };
    pFileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
    pFileDialog->SetFileTypeIndex(1);

    hr = pFileDialog->Show(NULL);
    // Get the selected path
    if (SUCCEEDED(hr)) {
        IShellItem* pItem = nullptr;
        if (SUCCEEDED(pFileDialog->GetResult(&pItem))) {
            LPWSTR pszFilePath = nullptr;
            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                // Convert LPWSTR to std::string
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                std::string filePath = converter.to_bytes(pszFilePath);

                std::wcout << pszFilePath << std::endl;

                if (ptrImageTransformer != nullptr)
                {
                    ptrImageTransformer->setSource(filePath);
					bResult = true;
                }

                // Frees the memory allocated for the string
                CoTaskMemFree(pszFilePath);
            }
            pItem->Release();
        }
    }

    pFileDialog->Release();
    CoUninitialize();
	return bResult;
}

void UIWindow::drawEditor(std::string& _sCurrentPage)
{
	cleanFrame();

	if (ptrImageTransformer != nullptr)
	{
		std::vector<cv::Mat> vecImages;
		int iWidth = iWindowWidth - 2;
		int iHeight = iWindowHeight - 2;

		ptrImageTransformer->getSplittedImg(vecImages, iWidth, iHeight, dImagePercentage, bAspectRatio);

		// In case of too large width with force aspect ratio 
		if (iHeight > iWindowHeight)
			iWindowHeight = iHeight + 2;
		resizeFrame();

		int iCalculatedWidth = iWidth * (dImagePercentage / 100);

		cvui::image(mFrame, 1, 1, vecImages[0]);
		cvui::image(mFrame, iCalculatedWidth, 1, vecImages[1]);


		cv::Point cursor = cvui::mouse();
		iCursorXOld = iCursorX;
		iCursorYOld = iCursorY;
		iCursorX = cursor.x;
		iCursorY = cursor.y;

		if (bDragging)
		{
			int iXDiff = iCursorX - iCursorXOld;
			int iYDiff = iCursorY - iCursorYOld;

			if (iEditorX + iXDiff >= 0 && iEditorX + iXDiff <= 500)
				iEditorX += iXDiff;

			if (iEditorY + iYDiff >= 0 && iEditorY + iYDiff <= 300)
				iEditorY += iYDiff;
		}

		// Draw face detection rectangle
		int iFaceCount = -1;
		if (bFaceDetection) {
			iFaceCount = ptrImageTransformer->detectFace(mFrame);
		}

		cvui::window(mFrame, iEditorX, iEditorY, iEditorWidth, iEditorHeight, "Editor");
		cvui::beginColumn(mFrame, iEditorX + 10, iEditorY + 30, iEditorWidth, iEditorHeight, 10);
		cv::Mat lena_face = cv::imread(getFullPath("./assets/drag.png"));
		cvui::image(mFrame, iEditorX + iEditorWidth - 20, iEditorY, lena_face);

		cvui::beginRow(-1, -1, 5);
		if (cvui::button("Back"))
		{
			_sCurrentPage = "menu";
		}

		if (cvui::button("Open"))
		{
			if (findPath())
				dBlurValue = 0.0;
			iMedianBlurValue = 3;
		}

		if (cvui::button("Save"))
		{
			ptrImageTransformer->save(getFullPath("./Output.jpg"), bFaceDetection);
		}
		cvui::endRow();

		bool bImageCleaned = false;
		int iTrackWidth = iEditorWidth - 20;

		// Blur trackbar
		cvui::text("Blur");
		if (cvui::trackbar(iTrackWidth, &dBlurValue, (double)0.0, (double)15.0))
		{
			ptrImageTransformer->clean();
			bImageCleaned = true;

			if (floor(dBlurValue + 0.5) > 1)
			{
				ptrImageTransformer->blur(floor(dBlurValue + 0.5));
			}
			else
			{
				ptrImageTransformer->blur(1);
			}

			//ptrImageTransformer->medianBlur(floor(dBlurValue + 0.5));
			fmt::println("Trackbar value: {}", dBlurValue);
		}

		// Median blur trackbar
		cvui::text("Median blur");
		if (cvui::trackbar(iTrackWidth, &iMedianBlurValue, 3, 15, 2))
		{
			if (!bImageCleaned)
				bImageCleaned = true;
			ptrImageTransformer->clean();

			fmt::println("Trackbar value: {}", iMedianBlurValue);
			ptrImageTransformer->medianBlur(iMedianBlurValue);
		}

		// Erode trackbar
		cvui::text("Erode");
		if (cvui::trackbar(iTrackWidth, &iErodeValue, 3, 15, 2)) {
			if (!bImageCleaned)
				bImageCleaned = true;
			ptrImageTransformer->clean();

			fmt::println("Trackbar value: {}", iErodeValue);
			ptrImageTransformer->erode(iErodeValue);
		}

		cvui::checkbox("Face Detection", &bFaceDetection);
		if (bFaceDetection && iFaceCount != -1) {
			if (iFaceCount > 0)
			{
				cvui::text(std::format("Faces detected: {}",iFaceCount));
			}
			else
			{
				cvui::text("No face detected !", 0.4, 0xEF1818);
			}
		}

		cvui::endColumn();


		cvui::trackbar(mFrame, iWindowWidth / 2 - 300, iWindowHeight - 40, 598, &dImagePercentage, (double)1.0, (double)99.0, 1, "%.1Lf", cvui::TRACKBAR_HIDE_LABELS);

		int iStatus = cvui::iarea(iEditorX + iEditorWidth - 20, iEditorY, 18, 18);
		switch (iStatus)
		{
		case cvui::CLICK:
			bDragging = false;
			break;
		case cvui::DOWN:
			bDragging = true;
			break;
		default:
			bDragging = false;
			break;
		}
	}
	cvui::imshow(sWindowName, mFrame);
}

void UIWindow::createButton() const
{
	cv::waitKey(0);
}

void UIWindow::destroyWindow() const
{
	cv::destroyWindow(sWindowName);
}
