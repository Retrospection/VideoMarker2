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

	cv::Rect RawToScreen(const cv::Rect& raw) const;

	cv::Rect ScreenToRaw(const cv::Rect& screen) const;

	cv::Rect MouseToScreen(const cv::Rect& mouse) const;

	cv::Rect ScreenToMouse(const cv::Rect& screen) const;

	cv::Rect ScreenToText(const cv::Rect& screen) const;
	
	cv::Rect TextToScreen(const cv::Rect& text) const;

	cv::Rect RawToRoi(const cv::Rect& raw) const;

	cv::Rect RoiToRaw(const cv::Rect& screen) const;

	cv::Rect RawToPictureBox(const cv::Rect& mouse) const;

	cv::Rect PictureBoxToRaw(const cv::Rect& screen) const;

	cv::Rect RoiToPictureBox(const cv::Rect& screen) const;

	cv::Rect PictureBoxToRoi(const cv::Rect& text) const;
//	std::function<cv::Rect(const cv::Rect)> GetTransform(int src, int dst) const;

public:

	cv::Rect Trans(const cv::Rect& input, int src, int dst) const;

};