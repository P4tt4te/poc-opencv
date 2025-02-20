#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class ImageTransformer {
private:
	std::string sInitialSource;
	cv::Mat mInitialImg;
	cv::Mat mImg;
public:
	ImageTransformer(std::string _sSource);
	void setSource(std::string _sSource);
	void getSplittedImg(std::vector<cv::Mat>& _vecImages, int& _width, int& _height, double _percentage, bool _bForceRatio = false);
	cv::Mat& getImg() { return mImg; };
	cv::Mat getInitialImg() const { return cv::imread(sInitialSource); };
	void save(std::string _sPath);
	// Image processing
	void clean();
	void blur(int _size);
	void medianBlur(int _size);
	void gaussianBlur(int _size) const;
};
