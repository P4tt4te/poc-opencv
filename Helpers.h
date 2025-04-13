#include <iostream>
#include <windows.h>
#include <stdio.h>

static std::string getFullPath(const char* _sPath) {
	std::string sFullPathName = "";
	char fullFilename[MAX_PATH];
	GetFullPathName(_sPath, MAX_PATH, fullFilename, nullptr);
	sFullPathName = fullFilename;
	return sFullPathName;
};

#pragma region Icon
static HWND _GetCvWindow(LPCSTR lpWndName) {
    HWND hWnd = (HWND)FindWindow(NULL, lpWndName);
    if (IsWindow(hWnd)) {
        HWND hParent = GetParent(hWnd);
        DWORD dwPid = 0;
        GetWindowThreadProcessId(hWnd, &dwPid);
        if (dwPid == GetCurrentProcessId()) {
            return hParent;
        }
    }

    return hWnd;
}

static void _SetCvWindowIcon(LPCSTR lpWndName) {
    HWND hWnd = _GetCvWindow(lpWndName);
    if (IsWindow(hWnd)) {
        char fullFilename[MAX_PATH];
        GetFullPathName("./assets/favicon.ico", MAX_PATH, fullFilename, nullptr);

        const size_t cSize = strlen(fullFilename) + 1;
        wchar_t* wc = new wchar_t[cSize];
        mbstowcs(wc, fullFilename, cSize);

        HANDLE hIcon = LoadImageW(NULL, wc, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
        SendMessageW(hWnd, (WPARAM)WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessageW(hWnd, (WPARAM)WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
}
#pragma endregion

static const std::vector<std::string> nanodetClassLabels =
{
    "person", "bicycle", "car", "motorcycle", "airplane", "bus",
    "train", "truck", "boat", "traffic light", "fire hydrant",
    "stop sign", "parking meter", "bench", "bird", "cat", "dog",
    "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe",
    "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat",
    "baseball glove", "skateboard", "surfboard", "tennis racket",
    "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl",
    "banana", "apple", "sandwich", "orange", "broccoli", "carrot",
    "hot dog", "pizza", "donut", "cake", "chair", "couch",
    "potted plant", "bed", "dining table", "toilet", "tv", "laptop",
    "mouse", "remote", "keyboard", "cell phone", "microwave",
    "oven", "toaster", "sink", "refrigerator", "book", "clock",
    "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
};
