#include <fmt/base.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class ImageTransformer {
private:
	std::string sInitialSource;
	cv::Mat mInitialImg;
	cv::Mat mInitialImgResized;
	cv::Mat mImg;
	cv::Ptr<cv::FaceDetectorYN> pFaceDetectorYN;
	cv::dnn::dnn4_v20240521::Net pReadNet;
	cv::Mat mFaces;
public:
	ImageTransformer(std::string _sSource);
	void setSource(std::string _sSource);
	void getSplittedImg(std::vector<cv::Mat>& _vecImages, int& _width, int& _height, double _percentage, bool _bForceRatio = false);
	cv::Mat& getImg() { return mImg; };
	cv::Mat getInitialImg() const { return cv::imread(sInitialSource); };
	void save(std::string _sPath, bool& _bWithFaces);
	// Image processing
	void clean();
	void blur(int _size);
	void medianBlur(int _size);
	void gaussianBlur(int _size) const;
	void erode(int _size);
	// Face detection
	int getFaceCount() { return mFaces.rows; };
	void detectFace(cv::Mat& _frame, bool _bNoResize = false);
	void drawFace(cv::Mat& _frame);
};
