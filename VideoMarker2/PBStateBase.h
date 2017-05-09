#pragma once

#include <opencv2/core/core.hpp>

class CPictureBox;
struct FaceInfo;
class IDrawable;

class CPBStateBase
{
public: 
	CPBStateBase(CPictureBox* ptr);
	virtual ~CPBStateBase();


	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	virtual void DrawActiveBox();
	
	virtual void DrawSavedFacesInfo();
	virtual void DrawSelectedFacesInfo();
	virtual void DrawHighlightFacesInfo();
	virtual void SetState(size_t nEditType);

protected:


	bool IsDrawing() const;
	void SetDrawing(bool bDrawing);
	bool IsInRoi(const CPoint& pt) const;
	
	void SetActivePoint(const cv::Point& pt, bool bBegin);
	void ResetActivePoints();

	bool HasChosenEditPoint() const;

	void MoveEditPoint(const cv::Point& pt);
	void AddFaceInfoToPictureBox(const FaceInfo& faceInfo);
	unsigned int ValidateFaceInfo();
	void HandleInvalidFaceInfo(unsigned int result);
	void SelectBox();

	cv::Rect Trans(const cv::Rect& rc, int src, int dst);

	bool GetUnsavedName(std::string& ret);
	bool GetActiveBox(cv::Rect& rc);

	void AddDrawables(IDrawable* drawable);

	bool GetActiveBoxEx(cv::Rect& rc);

	void ClearSelectEditPoint();

	bool GetActiveBoxFromActiveBoxManager(cv::Rect& rc);		// Get ActiveBox From ActiveBox Manager

	void SetStartActivePoint(const cv::Point& pt);
	void SetEndActivePoint(const cv::Point& pt);

	void UpdateDrawableActiveBox();


	void UpdateDrawableSavedFacesInfo();
	void UpdateDrawableSelectedFacesInfo();
	void UpdateDrawableHighlightFacesInfo();


	bool SelectEditPoint(const cv::Point& pt);

	static const bool ACTIVE_POINT_BEGIN = true;
	static const bool ACTIVE_POINT_END = false;

	void UpdateDlgFrameInfo();


private:
	CPictureBox* m_pPictureBox;
};

