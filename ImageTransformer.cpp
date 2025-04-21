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
	// Initialize ObjectDetector
	std::string od_modelPath = "./assets/object_detection_nanodet_2022nov.onnx";
	pReadNet = cv::dnn::readNet(od_modelPath);
	pReadNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	pReadNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
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


// #pragma region Face Detection
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

// #pragma endregion

// #pragma region Object Detection
cv::Mat reshapeIfNeeded(const cv::Mat& input)
{
	if (input.dims == 3)
	{
		return input.reshape(0, input.size[1]);
	}
	return input;
}

cv::Mat softmaxActivation(const cv::Mat& input)
{
	int regMax = 7;

	cv::Mat x_exp, x_sum, x_repeat_sum, result;
	cv::exp(input.reshape(0, input.total() / (regMax + 1)), x_exp);
	cv::reduce(x_exp, x_sum, 1, cv::REDUCE_SUM, CV_32F);
	cv::repeat(x_sum, 1, regMax + 1, x_repeat_sum);
	cv::divide(x_exp, x_repeat_sum, result);
	return result;
}

std::vector<cv::Mat> generateAnchors()
{
	std::vector<cv::Mat> anchorsMlvl;
	// Image shape is 416x416
	for (const int stride : { 8, 16, 32, 64 }) {
		int feat_h = 416 / stride;
		int feat_w = 416 / stride;

		std::vector<cv::Mat> anchors;

		for (int y = 0; y < feat_h; ++y)
		{
			for (int x = 0; x < feat_w; ++x)
			{
				float shift_x = x * stride;
				float shift_y = y * stride;
				float cx = shift_x + 0.5 * (stride - 1);
				float cy = shift_y + 0.5 * (stride - 1);
				cv::Mat anchor_point = (cv::Mat_<float>(2, 1) << cx, cy);
				anchors.push_back(anchor_point);
			}
		}
		cv::Mat anchors_mat;
		hconcat(anchors, anchors_mat);
		anchorsMlvl.push_back(anchors_mat);
	}
	return anchorsMlvl;
}

cv::Mat applyProjection(cv::Mat& input)
{
	int regMax = 7;
	cv::Mat repeat_project;
	cv::Mat project = cv::Mat::zeros(1, regMax + 1, CV_32F);
	repeat(project, input.rows, 1, repeat_project);
	multiply(input, repeat_project, input);
	reduce(input, input, 1, cv::REDUCE_SUM, CV_32F);
	cv::Mat projection = input.col(0).clone();
	return projection.reshape(0, projection.total() / 4);
}

void preNMS(cv::Mat& anchors, cv::Mat& bbox_pred, cv::Mat& cls_score, const int nms_pre = 1000)
{
	cv::Mat max_scores;
	reduce(cls_score, max_scores, 1, cv::REDUCE_MAX);

	cv::Mat indices;
	sortIdx(max_scores.t(), indices, cv::SORT_DESCENDING);

	cv::Mat indices_float = indices.colRange(0, nms_pre);
	cv::Mat selected_anchors, selected_bbox_pred, selected_cls_score;
	for (int j = 0; j < indices_float.cols; ++j)
	{
		selected_anchors.push_back(anchors.row(indices_float.at<int>(j)));
		selected_bbox_pred.push_back(bbox_pred.row(indices_float.at<int>(j)));
		selected_cls_score.push_back(cls_score.row(indices_float.at<int>(j)));
	}

	anchors = selected_anchors;
	bbox_pred = selected_bbox_pred;
	cls_score = selected_cls_score;
}

void clipBoundingBoxes(cv::Mat& x1, cv::Mat& y1, cv::Mat& x2, cv::Mat& y2)
{
	cv::Mat zeros = cv::Mat::zeros(x1.size(), x1.type());
	// Image shape is 416x416
	x1 = min(max(x1, zeros), cv::Scalar(416 - 1));
	y1 = min(max(y1, zeros), cv::Scalar(416 - 1));
	x2 = min(max(x2, zeros), cv::Scalar(416 - 1));
	y2 = min(max(y2, zeros), cv::Scalar(416 - 1));
}

cv::Mat calculateBoundingBoxes(const cv::Mat& anchors, const cv::Mat& bbox_pred)
{
	cv::Mat x1 = anchors.col(0) - bbox_pred.col(0);
	cv::Mat y1 = anchors.col(1) - bbox_pred.col(1);
	cv::Mat x2 = anchors.col(0) + bbox_pred.col(2);
	cv::Mat y2 = anchors.col(1) + bbox_pred.col(3);

	clipBoundingBoxes(x1, y1, x2, y2);

	cv::Mat bboxes;
	hconcat(std::vector<cv::Mat>{x1, y1, x2, y2}, bboxes);

	return bboxes;
}

std::vector<cv::Rect2d> bboxMatToRect2d(const cv::Mat& bboxes)
{
	cv::Mat bboxes_wh(bboxes.clone());
	bboxes_wh.colRange(2, 4) = bboxes_wh.colRange(2, 4) -= bboxes_wh.colRange(0, 2);
	std::vector<cv::Rect2d> boxesXYXY;
	for (size_t i = 0; i < bboxes_wh.rows; i++)
	{
		boxesXYXY.emplace_back(bboxes.at<float>(i, 0),
			bboxes.at<float>(i, 1),
			bboxes.at<float>(i, 2),
			bboxes.at<float>(i, 3));
	}
	return boxesXYXY;
}

void ImageTransformer::detectObject(cv::Mat& _frame, bool _bNoResize)
{
	cv::Mat& i1 = _bNoResize ? mImg : mInitialImgResized;

	cv::dnn::Image2BlobParams paramNanodet;
	paramNanodet.datalayout = cv::dnn::DNN_LAYOUT_NCHW;
	paramNanodet.ddepth = CV_32F;
	paramNanodet.mean = cv::Scalar(103.53, 116.28, 123.675);
	paramNanodet.scalefactor = cv::Scalar(49, 52, 49);
	paramNanodet.size = cv::Size(416, 416);

	cv::Mat blob;
	cv::dnn::blobFromImageWithParams(mInitialImgResized, blob, paramNanodet);
	pReadNet.setInput(blob);

	std::vector<cv::Mat> preds;
	pReadNet.forward(preds, pReadNet.getUnconnectedOutLayersNames());

	// TODO: write objects & init parameters one time
	std::vector<cv::Mat> cls_scores, bbox_preds;
	for (size_t i = 0; i < preds.size(); i += 2)
	{
		cls_scores.push_back(preds[i]);
		bbox_preds.push_back(preds[i + 1]);
	}

	std::vector<cv::Mat> bboxes_mlvl;
	std::vector<cv::Mat> scores_mlvl;

	std::vector<int> strides = { 8, 16, 32, 64 };
	std::vector<cv::Mat> anchorsMlvl = generateAnchors();

	for (size_t i = 0; i < strides.size(); ++i)
	{
		if (i >= cls_scores.size() || i >= bbox_preds.size()) continue;
		// Extract necessary data
		int stride = strides[i];
		cv::Mat cls_score = reshapeIfNeeded(cls_scores[i]);
		cv::Mat bbox_pred = reshapeIfNeeded(bbox_preds[i]);
		cv::Mat anchors = anchorsMlvl[i].t();

		// Softmax activation, projection, and calculate bounding boxes
		bbox_pred = softmaxActivation(bbox_pred);
		bbox_pred = applyProjection(bbox_pred);
		bbox_pred = stride * bbox_pred;

		const int nms_pre = 1000;
		if (nms_pre > 0 && cls_score.rows > nms_pre)
		{
			preNMS(anchors, bbox_pred, cls_score, nms_pre);
		}

		cv::Mat bboxes = calculateBoundingBoxes(anchors, bbox_pred);


		bboxes_mlvl.push_back(bboxes);
		scores_mlvl.push_back(cls_score);
	}
	cv::Mat bboxes;
	cv::Mat scores;
	vconcat(bboxes_mlvl, bboxes);
	vconcat(scores_mlvl, scores);

	std::vector<cv::Rect2d> boxesXYXY = bboxMatToRect2d(bboxes);
	std::vector<int> classIds;
	std::vector<float> confidences;
	for (size_t i = 0; i < scores.rows; ++i)
	{
		cv::Point maxLoc;
		minMaxLoc(scores.row(i), nullptr, nullptr, nullptr, &maxLoc);
		classIds.push_back(maxLoc.x);
		confidences.push_back(scores.at<float>(i, maxLoc.x));
	}

	std::vector<int> indices;
	float probThreshold = 0.35;
	float iouThreshold = 0.6;
	cv::dnn::NMSBoxesBatched(boxesXYXY, confidences, classIds, probThreshold, iouThreshold, indices);
	cv::Mat result(int(indices.size()), 6, CV_32FC1);
	int row = 0;
	for (auto idx : indices)
	{
		bboxes.rowRange(idx, idx + 1).copyTo(result(cv::Rect(0, row, 4, 1)));
		result.at<float>(row, 4) = confidences[idx];
		result.at<float>(row, 5) = static_cast<float>(classIds[idx]);
		row++;
	}
	if (indices.size() == 0)
	{
		return;
	} 
	cv::imshow("Object Detection", result);
	return;
}

void ImageTransformer::drawObject(cv::Mat& _frame) {
	


}
// #pragma endregion
