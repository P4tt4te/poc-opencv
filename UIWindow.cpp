#include "UIWindow.h"
#include "Windows.h"
#include <shobjidl.h>
#include "WinUser.h"
#include "shellapi.h"
#include "shlobj_core.h"
#include <opencv2/highgui.hpp>
#include "cvui.h"

UIWindow::UIWindow(std::string _sWindowName)
{
	sWindowName = _sWindowName;
}

void UIWindow::createWindow() const
{
	cvui::init(sWindowName);
}

void UIWindow::drawMenu(cv::Mat& _frame) const
{
	_frame = cv::Scalar(49, 52, 49);
	//cvui::beginColumn(_frame, 10, 20);


		cvui::text(_frame, 10, 10,"IEA");
		if (cvui::button(_frame, 100, 40, "Button")) {
			// button was clicked
		}
	//cvui::endColumn();

	// Show window content
	cvui::imshow(sWindowName, _frame);
}

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
				//fmt::print("Image sélectionnée :");
				std::wcout << pszFilePath << std::endl;
				// Frees the memory allocated for the string
				CoTaskMemFree(pszFilePath);
			}
			pItem->Release();
		}
	}

	pFileDialog->Release();
	CoUninitialize();
}

void UIWindow::drawEditor() const
{

}

void UIWindow::showImage(const cv::Mat& _img) const
{
	cv::Mat frame = cv::Mat(cv::Size(400, 200), CV_8UC3);
	frame = cv::Scalar(49, 52, 49);
	cvui::text(frame, 10, 15, "Hello world!");

	// Show window content
	cvui::imshow(sWindowName, frame);
	//cv::imshow(sWindowName, _img);
	cv::waitKey(0);
}

void UIWindow::createButton() const
{
	cv::waitKey(0);
}

void UIWindow::destroyWindow() const
{
	cv::destroyWindow(sWindowName);
}
