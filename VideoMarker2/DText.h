#include "IDrawable.h"


#include <opencv2/core/core.hpp>
#include <string>

class DText:public IDrawable
{
public:

	DText(const std::string& str,const cv::Point& org, const cv::Scalar& color)
		: m_str(str), m_color(color), m_org(org){}


	virtual void Draw(cv::Mat& mat) override
	{
		cv::putText(mat, m_str, m_org, cv::FONT_HERSHEY_COMPLEX, 1, m_color);
	}



private:
	const std::string m_str;
	const cv::Scalar m_color;
	const cv::Point m_org;
};