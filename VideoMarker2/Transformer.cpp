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
	if ((source.width / source.height) > (target.width / target.height))
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

cv::Rect Transformer::RawToScreen(const cv::Rect& raw) const
{
	return std::move(raw / m_Ratio);
}

cv::Rect Transformer::ScreenToRaw(const cv::Rect& screen) const
{
	return std::move(screen * m_Ratio);
}

cv::Rect Transformer::MouseToScreen(const cv::Rect& mouse) const
{
	return{ mouse.x - m_Roi.x, mouse.y - m_Roi.y, mouse.width, mouse.height };
}

cv::Rect Transformer::ScreenToMouse(const cv::Rect& screen) const
{
	return{ screen.x + m_Roi.x, screen.y + m_Roi.y, screen.width, screen.height };
}

cv::Rect Transformer::ScreenToText(const cv::Rect& screen) const
{
	return screen*m_Ratio;
}

cv::Rect Transformer::TextToScreen(const cv::Rect& text) const
{
	return text / m_Ratio;
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
	case 0*10+1:
		return RawToRoi(input);
	case 1*10+0:
		return RoiToRaw(input);
	case 0*10+2:
		return RawToPictureBox(input);
	case 2*10+0:
		return PictureBoxToRaw(input);
	case 1*10+2:
		return RoiToPictureBox(input);
	case 2*10+1:
		return PictureBoxToRoi(input);
	default:
		assert(false);
		return input;
	}
}

// std::function<cv::Rect(const cv::Rect)> Transformer::GetTransform(int src, int dst) const
// {
// 	return m_Transforms[src][dst];
// }

cv::Rect Transformer::DefaultTransform(const cv::Rect& input) const
{
	return input;
}

cv::Rect Transformer::RawToRoi(const cv::Rect& raw) const
{
	return raw / m_Ratio;
}

cv::Rect Transformer::RoiToRaw(const cv::Rect& screen) const
{
	return screen * m_Ratio;

}

cv::Rect Transformer::RawToPictureBox(const cv::Rect& mouse) const
{
	return{};

}

cv::Rect Transformer::PictureBoxToRaw(const cv::Rect& screen) const
{
	return{};

}

cv::Rect Transformer::RoiToPictureBox(const cv::Rect& screen) const
{
	return{};

}

cv::Rect Transformer::PictureBoxToRoi(const cv::Rect& text) const
{
	return{};

}
