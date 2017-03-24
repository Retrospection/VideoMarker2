#pragma once
#include <vector>
#include <string>
#include <sstream>

#include <opencv2/core/core.hpp>

struct FaceInfo
{
	std::string strPersonName;
	cv::Rect box;

	std::string toString()
	{
		std::stringstream ss;
		ss << box.x << " " << box.y << " " << box.width << " " << box.height << " " << strPersonName;
		return ss.str();
	}
};

struct FrameInfo
{
	std::vector<FaceInfo> facesInfo;

	std::string toString() const
	{
		std::stringstream ss;
		ss << facesInfo.size() << " ";
		for (auto faceinfo: facesInfo)
		{
			ss << faceinfo.toString() << " ";
		}
		return ss.str().substr(0, ss.str().size() - 1);
	}

};

