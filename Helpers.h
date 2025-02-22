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