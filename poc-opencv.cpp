#include <fmt/base.h>
#include "poc-opencv.h"
#include "ImageTransformer.h"

int main()
{
	fmt::print("Hello, {}!\n", "world");
	ImageTransformer imgTrans("C:/Users/Edward/Pictures/chaudok.png");
	imgTrans.blur();

	return 0;
}
