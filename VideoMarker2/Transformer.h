#pragma once

#include "stdafx.h"

#include <opencv2/core/core.hpp>

#include <functional>


class Transformer
{
private:
	Transformer(cv::Rect roi, double ratio);

	cv::Rect m_Roi;
	double m_Ratio;


public:
	static Transformer Make(cv::Size target, cv::Size source);
	static Transformer Default();
	

	enum Coordinate
	{
		Raw = 0,
		Roi = 1,
		PictureBox = 2
	};

	cv::Rect GetRoiRect()const;

//private:

	cv::Rect DefaultTransform(const cv::Rect& input) const;

	cv::Rect RawToRoi(const cv::Rect& input) const;

	cv::Rect RoiToRaw(const cv::Rect& input) const;

	cv::Rect RawToPictureBox(const cv::Rect& input) const;

	cv::Rect PictureBoxToRaw(const cv::Rect& input) const;

	cv::Rect RoiToPictureBox(const cv::Rect& input) const;

	cv::Rect PictureBoxToRoi(const cv::Rect& input) const;

public:

	cv::Rect Trans(const cv::Rect& input, int src, int dst) const;

};