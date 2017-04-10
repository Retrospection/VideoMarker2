#pragma once

#include <opencv2/core/core.hpp>

#include <vector>

class Transformer;
class IDrawable;

class CActiveBoxManager
{
public:
	CActiveBoxManager(Transformer* ptr);
	~CActiveBoxManager();

public:
	void SetStartActivePoint(const cv::Point& pt);
	void SetEndActivePoint(const cv::Point& pt);
	bool GetActiveBoxInRoi(cv::Rect& activeBox) const;

	void ResetActiveBox();

	void UpdateDrawableActiveBox(std::vector<IDrawable*>& toBeUpdated);

public:
	static const cv::Point INIT_POINT;


private:
	bool m_bChanged;
	cv::Point m_ActivePoints[2];
	Transformer* m_pTrans;
};

