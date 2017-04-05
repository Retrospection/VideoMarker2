#pragma once

#include <vector>
#include <opencv2/core/core.hpp>

class RectEx : public cv::Rect
{
public:
	cv::Point TopLeft()	const		{ return{ x, y }; }
	cv::Point TopMid() const		{ return{ x + width / 2, y }; }
	cv::Point TopRight() const		{ return{ x + width, y }; }
	cv::Point MidLeft() const		{ return{ x, y + height / 2 }; }
	cv::Point MidRight() const		{ return{ x + width, y + height / 2 }; }
	cv::Point BottomLeft() const	{ return{ x, y + height }; }
	cv::Point BottomMid() const		{ return{ x + width / 2, y + height }; }
	cv::Point BottomRight() const	{ return{ x + width, y + height }; }

	void TopLeft(const cv::Point& pt)		{ ThisRect() = { pt, br() }; }
	void TopMid(const cv::Point& pt)		{ ThisRect() = { cv::Point{ x, pt.y }, br() }; }
	void MidLeft(const cv::Point& pt)		{ ThisRect() = { cv::Point{ pt.x, y }, br() }; }
	void BottomRight(const cv::Point& pt)	{ ThisRect() = { tl(), pt }; }
	void MidRight(const cv::Point& pt)		{ ThisRect() = { tl(), cv::Point{ pt.x, br().y } }; }
	void BottomMid(const cv::Point& pt)		{ ThisRect() = { tl(), cv::Point{ br().x, pt.y } }; }
	void TopRight(const cv::Point& pt)		{ ThisRect() = { cv::Point{ x, pt.y }, cv::Point{ pt.x, br().y } }; }
	void BottomLeft(const cv::Point& pt)	{ ThisRect() = { cv::Point{ pt.x, y }, cv::Point{ br().x, pt.y } }; }

	void Move(int nType, const cv::Point& offset)
	{
		switch (nType)
		{
		case 0: { TopLeft(TopLeft() + offset); break; }
		case 1: { TopMid(TopMid() + offset); break; }
		case 2: { TopRight(TopRight() + offset); break; }
		case 3: { MidLeft(MidLeft() + offset); break; }
		case 4: { MidRight(MidRight() + offset); break; }
		case 5: { BottomLeft(BottomLeft() + offset); break; }
		case 6: { BottomMid(BottomMid() + offset); break; }
		case 7: { BottomRight(BottomRight() + offset); break; }
		default: break;
		}
	}
private:
	cv::Rect& ThisRect()
	{
		return *(cv::Rect*)(this);
	}
};




