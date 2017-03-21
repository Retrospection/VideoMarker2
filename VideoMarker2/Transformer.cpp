#include "stdafx.h"

#include "Transformer.h"
#include <cassert>

cv::Rect operator /(const cv::Rect& rc, double ratio)
{
	return{ static_cast<int>(rc.x / ratio), static_cast<int>(rc.y / ratio), static_cast<int>(rc.width / ratio), static_cast<int>(rc.height / ratio) };
}

cv::Rect operator *(const cv::Rect& rc, double ratio)
{
	return{ static_cast<int>(rc.x * ratio), static_cast<int>(rc.y * ratio), static_cast<int>(rc.width * ratio), static_cast<int>(rc.height * ratio) };
}

Transformer::Transformer(cv::Rect roi, double ratio)
	:m_Ratio(ratio), m_Roi(roi)
{

}

Transformer Transformer::Make(cv::Size target, cv::Size source)
{
	int width = 0;
	int height = 0;
	int x = 0;
	int y = 0;
	double ratio = 0.0;
	if (((double)source.width / source.height) > ((double)target.width / target.height))
	{
		width = target.width;
		ratio = ((double)source.width) / target.width;
		height = source.height / ratio;
		x = 0;
		y = ((target.height / 2) - (height / 2));
	}
	else
	{
		height = target.height;
		ratio = ((double)source.height) / target.height;
		width = source.width / ratio;
 		y = 0;
		x = ((target.width / 2) - (width / 2));
	}
	return Transformer({ x, y, width, height }, ratio);
}

cv::Rect Transformer::GetRoiRect() const
{
	return m_Roi;
}

Transformer Transformer::Default()
{
	return Make({ 1, 1 }, { 1, 1 });
}

cv::Rect Transformer::Trans(const cv::Rect& input, int src, int dst) const
{
	assert(src >= 0 && src <= 2 && dst >= 0 && dst <= 2);
	if (src == dst)
	{
		return input;
	}
	switch (src * 10 + dst)
	{
	case Raw * 10 + Roi:
		return RawToRoi(input);
	case Roi * 10 + Raw:
		return RoiToRaw(input);
	case Raw * 10 + PictureBox:
		return RawToPictureBox(input);
	case PictureBox * 10 + Raw:
		return PictureBoxToRaw(input);
	case Roi * 10 + PictureBox:
		return RoiToPictureBox(input);
	case PictureBox * 10 + Roi:
		return PictureBoxToRoi(input);
	default:
		assert(false);
		return input;
	}
}


cv::Rect Transformer::DefaultTransform(const cv::Rect& input) const
{
	return input;
}

cv::Rect Transformer::RawToRoi(const cv::Rect& input) const
{
	return input / m_Ratio;
}

cv::Rect Transformer::RoiToRaw(const cv::Rect& input) const
{
	return input * m_Ratio;

}

cv::Rect Transformer::RawToPictureBox(const cv::Rect& input) const
{
	return RoiToPictureBox(RawToRoi(input));

}

cv::Rect Transformer::PictureBoxToRaw(const cv::Rect& input) const
{
	return RoiToRaw(PictureBoxToRoi(input));

}

cv::Rect Transformer::RoiToPictureBox(const cv::Rect& input) const
{
	return{input.x + m_Roi.x, input.y + m_Roi.y, input.width, input.height};

}

cv::Rect Transformer::PictureBoxToRoi(const cv::Rect& input) const
{

	return{input.x - m_Roi.x, input.y - m_Roi.y, input.width, input.height};
}
