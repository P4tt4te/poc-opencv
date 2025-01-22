#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class ImageTransformer {
public:
	ImageTransformer(std::string _sSource);
	void blur() const;
private:
	std::string sInitialSource;
};
