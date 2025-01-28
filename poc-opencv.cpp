#include "ImageTransformer.h"
#include "UIWindow.h"
#include <windows.h>
#include <stdio.h>

HHOOK _k_hook;
LRESULT __stdcall k_Callback1(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    //a key was pressed
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {
        switch (key->vkCode)
        {
        case VK_F1:
            puts("F1");
            break;
        case VK_F2:
            puts("F2");
            break;
        case VK_F3:
            puts("F3");
            break;
        case 81:
            puts("Q");
            PostMessage(HWND_BROADCAST, 0, key->vkCode, 0);
            break;
        };
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main()
{
	fmt::println("Welcome to OpenCV demo\n");

	ImageTransformer imgTrans("C:/Users/edwar/Pictures/Acer/Acer_Wallpaper_02_5000x2813.jpg");
	int size = 9;
	imgTrans.blur(size);
	imgTrans.medianBlur(size);
	imgTrans.gaussianBlur(size);

	//UIWindow window("Image");

	//window.createButton();
	//window.findPath();
	//window.showImage(imgTrans.getImg());

    _k_hook = SetWindowsHookExA(WH_KEYBOARD_LL, k_Callback1, NULL, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        fmt::println("message");
    }
    if (_k_hook)
        fmt::println("UNHOOK");
        UnhookWindowsHookEx(_k_hook);
    return TRUE;

	return 0;
}
