#include "UIWindow.h"
#include "Windows.h"
#include <shobjidl.h>
#include "WinUser.h"
#include "shellapi.h"
#include "shlobj_core.h"
//#pragma comment(lib, "shell32")

UIWindow::UIWindow(std::string _sWindowName)
{
	sWindowName = _sWindowName;

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

void UIWindow::showImage(const cv::Mat& _img) const
{
	cv::imshow(sWindowName, _img);
	cv::waitKey(0);
}

void UIWindow::destroyWindow() const
{
	cv::destroyWindow(sWindowName);
}
