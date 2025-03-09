#include "ImageTransformer.h"

ImageTransformer::ImageTransformer(std::string _sSource) {
	sInitialSource = _sSource;
	std::replace(sInitialSource.begin(), sInitialSource.end(), '\\', '/');
	mImg = cv::imread(sInitialSource, cv::IMREAD_COLOR);
	mInitialImg = mImg;
	
	// Initialize FaceDetectorYN
	std::string fd_modelPath = "./assets/face_detection_yunet_2023mar.onnx";
	float scoreThreshold = 0.9;
	float nmsThreshold = 0.3;
	int topK = 5000;

	bool save = false;
	float scale = 1.0;

	double cosine_similar_thresh = 0.363;
	double l2norm_similar_thresh = 1.128;

	pFaceDetectorYN = cv::FaceDetectorYN::create(fd_modelPath, "", cv::Size(320, 320), scoreThreshold, nmsThreshold, topK);
}

void ImageTransformer::setSource(std::string _sSource) {
	sInitialSource = _sSource;
	std::replace(sInitialSource.begin(), sInitialSource.end(), '\\', '/');
	mInitialImg = cv::imread(sInitialSource);
	mImg = mInitialImg;
	mFaces = cv::Mat();
}

void ImageTransformer::getSplittedImg(std::vector<cv::Mat>& _vecImages,int& _width, int& _height, double _percentage, bool _bForceRatio)
{
	cv::Mat filteredImage = getImg();
	cv::Mat initialImage = getInitialImg();

	if (_bForceRatio)
	{
		double dRatio = static_cast<double>(initialImage.rows) / static_cast<double>(initialImage.cols);

		_height = _width * dRatio;
	}

	cv::Size fitSize = cv::Size(_width, _height);

	cv::Mat filteredImageResized;
	resize(filteredImage, filteredImageResized, fitSize, cv::INTER_LINEAR);
	cv::Mat initialImageResized;
	resize(initialImage, initialImageResized, fitSize, cv::INTER_LINEAR);
	mInitialImgResized = initialImageResized;

	int iCalculatedWidth = _width * (_percentage / 100);
	int iRestCalculatedWidth = _width - iCalculatedWidth;
	cv::Rect initialRect(0, 0, iCalculatedWidth, _height);
	cv::Rect filteredRect(iCalculatedWidth, 0, iRestCalculatedWidth, _height);

	_vecImages.push_back(initialImageResized(initialRect));
	_vecImages.push_back(filteredImageResized(filteredRect));
}

void ImageTransformer::save(std::string _sPath, bool& _bWithFaces)
{
	cv::Mat imgWithFaces = mImg.clone();
	if (_bWithFaces)
		detectFace(imgWithFaces, true);

	cv::imwrite(_sPath,imgWithFaces);
}

void ImageTransformer::clean()
{
	mInitialImg = getInitialImg();
}

void ImageTransformer::blur(int _size) {
	mImg = mInitialImg;
	cv::blur(mInitialImg, mImg, cv::Size(_size, _size));
}

void ImageTransformer::medianBlur(int _size) {
	mImg = mInitialImg;

	if (_size % 2 != 1)
		_size++;

	cv::medianBlur(mInitialImg, mImg, _size);
}

void ImageTransformer::gaussianBlur(int _size) const {
	cv::Mat imgBlur = mImg;
	cv::GaussianBlur(imgBlur, mImg, cv::Size(_size, _size), 0);
}

void ImageTransformer::erode(int _size) {
	mImg = mInitialImg;
	cv::Mat kernel = cv::getStructuringElement(2, cv::Size(_size, _size));
	cv::erode(mImg, mImg, kernel);
}

void ImageTransformer::detectFace(cv::Mat& _frame, bool _bNoResize)
{
	int thickness = 2;
	// Set input size before inference
	cv::Mat& i1 = _bNoResize ? mImg : mInitialImgResized;

	pFaceDetectorYN->setInputSize(i1.size());

	pFaceDetectorYN->detect(i1, mFaces);
}

void ImageTransformer::drawFace(cv::Mat& _frame)
{
	int thickness = 2;
	if (mFaces.rows < 1)
	{
		return;
	}

	for (int i = 0; i < mFaces.rows; i++) 
	{
		// Draw bounding box
		cv::rectangle(_frame, cv::Rect2i(int(mFaces.at<float>(i, 0)), int(mFaces.at<float>(i, 1)), int(mFaces.at<float>(i, 2)), int(mFaces.at<float>(i, 3))), cv::Scalar(0, 255, 0), thickness);
		// Draw landmarks
		cv::circle(_frame, cv::Point2i(int(mFaces.at<float>(i, 4)), int(mFaces.at<float>(i, 5))), 2, cv::Scalar(255, 0, 0), thickness);
		cv::circle(_frame, cv::Point2i(int(mFaces.at<float>(i, 6)), int(mFaces.at<float>(i, 7))), 2, cv::Scalar(0, 0, 255), thickness);
		cv::circle(_frame, cv::Point2i(int(mFaces.at<float>(i, 8)), int(mFaces.at<float>(i, 9))), 2, cv::Scalar(0, 255, 0), thickness);
		cv::circle(_frame, cv::Point2i(int(mFaces.at<float>(i, 10)), int(mFaces.at<float>(i, 11))), 2, cv::Scalar(255, 0, 255), thickness);
		cv::circle(_frame, cv::Point2i(int(mFaces.at<float>(i, 12)), int(mFaces.at<float>(i, 13))), 2, cv::Scalar(0, 255, 255), thickness);
	}
}
