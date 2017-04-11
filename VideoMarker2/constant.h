#pragma once



enum FACEINFO_ERROR
{
	VALID = 0,
	BOX_SMALL_ERROR = 1,
	NAME_EXIST_ERROR = 2,
	IOU_ERROR = 3
};


enum OPEN_PROJECT_ERROR
{
	SUCCESS = 0,
	LOAD_PROJECT_ERROR = 1,
	OPEN_VIDEO_ERROR = 2,
	OPEN_TEXTFILE_ERROR = 3
};


const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };
const cv::Scalar Blue{ 255, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;
const cv::Scalar ColorSelected = Blue;
