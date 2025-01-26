#include <fmt/base.h>
#include "poc-opencv.h"
#include "ImageTransformer.h"
#include "UIWindow.h"

int main()
{
	fmt::print("Hello, {}!\n", "world");
	ImageTransformer imgTrans("C:/Users/Edward/Pictures/chaudok.png");
	const int size = 50;
	imgTrans.blur(size);
	//imgTrans.medianBlur(size);
	//imgTrans.gaussianBlur(size);

	UIWindow window("Image");
	window.showImage(imgTrans.getImg());


	return 0;
}
