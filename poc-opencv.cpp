#include "ImageTransformer.h"
#include "UIWindow.h"

int main()
{
	ImageTransformer imgTrans("C:/Users/edwar/Pictures/mat.png");
	int size = 9;
	imgTrans.blur(size);
	imgTrans.medianBlur(size);
	imgTrans.gaussianBlur(size);

	UIWindow window("Image");

	window.findPath();
	window.showImage(imgTrans.getImg());


	return 0;
}
