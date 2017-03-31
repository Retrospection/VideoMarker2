#include "stdafx.h"
#include "EditBox.h"

#include <iostream>

class RectEx
	: cv::Rect
{
public:
	cv::Point TopLeft()	const		{ return { x,				y }; }
	cv::Point TopMid() const		{ return { x + width / 2,	y }; }
	cv::Point TopRight() const		{ return { x + width,		y }; }
	cv::Point MidLeft() const		{ return { x,				y + height / 2}; }
	cv::Point MidRight() const		{ return { x + width,		y + height / 2}; }
	cv::Point BottomLeft() const	{ return { x,				y + height }; }
	cv::Point BottomMid() const		{ return { x + width / 2,	y + height }; }
	cv::Point BottomRight() const	{ return { x + width,		y + height }; }

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
		case 0: { TopLeft(TopLeft() + offset); break;}
		case 1: { TopMid(TopMid() + offset); break;}
		case 2: { TopRight(TopRight() + offset); break;}
		case 3: { MidLeft(MidLeft() + offset); break;}
		case 4: { MidRight(MidRight() + offset); break;}
		case 5: { BottomLeft(BottomLeft() + offset); break;}
		case 6: { BottomMid(BottomMid() + offset); break;}
		case 7: { BottomRight(BottomRight() + offset); break;}
		default: break;
		}
	}
private:
	cv::Rect& ThisRect()
	{
		return *(cv::Rect*)(this);
	}
};

CEditBox::CEditBox(cv::Rect& rc)
	:m_Box(*reinterpret_cast<RectEx*>(&rc))
{
	UpdateEditRects();
}


CEditBox::~CEditBox()
{
}

void CEditBox::UpdateEditRects()
{
	m_EditRects.clear();
	auto points = CalculateEditPoints();
	for (auto& pt : points)
	{
		m_EditRects.push_back(GetEditMarkRect(pt));
	}

}

const cv::Rect& CEditBox::GetBox() const
{
	return (cv::Rect&)m_Box;
}

const std::vector<cv::Rect>& CEditBox::GetEditRects() const
{
	return m_EditRects;
}

int CEditBox::Hit(const cv::Point& point) const
{
	for (size_t i = 0; i < m_EditRects.size(); ++i)
	{
		if (m_EditRects[i].contains(point))
		{
			return i;
		}
	}

	return -1;
}

std::vector<cv::Point> CEditBox::CalculateEditPoints()
{
	return { m_Box.TopLeft(), m_Box.TopMid(), m_Box.TopRight(),
		m_Box.MidLeft(), m_Box.MidRight(),
		m_Box.BottomLeft(), m_Box.BottomMid(), m_Box.BottomRight()
	};
}

cv::Rect CEditBox::GetEditMarkRect(const cv::Point& editPoint)
{
	int hw = 20;
	int hh = hw;
	cv::Point tl(editPoint.x - hw, editPoint.y - hh);
	cv::Point br(editPoint.x + hw, editPoint.y + hh);
	return{ tl, br };
}

void CEditBox::UpdateLocation(int nType, const cv::Point& offset)
{
	m_Box.Move(nType, offset);
	UpdateEditRects();
}
