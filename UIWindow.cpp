#include "UIWindow.h"
#include "Windows.h"
#include <shobjidl.h>
#include "WinUser.h"
#include "shellapi.h"
#include "shlobj_core.h"
#include <opencv2/highgui.hpp>
#include "cvui.h"
#include <fmt/base.h>
//#include "ImageTransformer.h"

UIWindow::UIWindow(std::string _sWindowName, ImageTransformer* _imageTransformer)
{
	sWindowName = _sWindowName;
	ptrImageTransformer = _imageTransformer;
}

void UIWindow::createWindow() const
{
	cvui::init(sWindowName);
}

void UIWindow::drawMenu(cv::Mat& _frame, std::string& _sCurrentPage) const 
{

	_frame = cv::Scalar(49, 52, 49);
	cvui::beginColumn(_frame, 10, 20);


	cvui::text("IEA");
	if (cvui::button(100, 40, "Open editor")) {
		// button was clicked
		_sCurrentPage = "editor";
	}
	cvui::endColumn();

	cvui::imshow(sWindowName, _frame);
}

#include <codecvt>

// ... other includes

void UIWindow::findPath()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (!SUCCEEDED(hr)) {
        fmt::report_error("Error: CoInitializeEx failed");
    }

    IFileDialog* pFileDialog = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

    if (!SUCCEEDED(hr)) {
        fmt::report_error("Error: CoCreateInstance failed");
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
                }

                // Frees the memory allocated for the string
                CoTaskMemFree(pszFilePath);
            }
            pItem->Release();
        }
    }

    pFileDialog->Release();
    CoUninitialize();
}

void UIWindow::drawEditor(cv::Mat& _frame, std::string& _sCurrentPage)
{
	_frame = cv::Scalar(49, 52, 49);

	if (ptrImageTransformer != nullptr)
	{
		cv::Mat a = ptrImageTransformer->getImg();

		// Resize to fit window
		int down_width = 598;
		int down_height = 398;
		cv::Mat resized_down;
		resize(a, resized_down, cv::Size(down_width, down_height), cv::INTER_LINEAR);

		cvui::image(_frame, 1, 1, resized_down);
	}


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
	

	cvui::window(_frame, iEditorX, iEditorY, iEditorWidth, iEditorHeight, "Editor");
	cvui::beginColumn(_frame, iEditorX + 10, iEditorY + 30, iEditorWidth, iEditorHeight, 10);
		cvui::text("Current file : ");

		if (cvui::button("Open file"))
		{
			findPath();
		}

		if (cvui::button("Go Back"))
		{
			_sCurrentPage = "menu";
		}
	cvui::endColumn();

	int iStatus = cvui::iarea(iEditorX, iEditorY, iEditorWidth, iEditorHeight);
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

	cvui::imshow(sWindowName, _frame);
}

void UIWindow::createButton() const
{
	cv::waitKey(0);
}

void UIWindow::destroyWindow() const
{
	cv::destroyWindow(sWindowName);
}
