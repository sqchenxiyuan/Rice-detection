#pragma once
class CWatcher
{
public:
	CWatcher(Mat background);
	~CWatcher();
	vector<Mat> getBlock(Mat img);
private:
	Mat m_background;
	Mat multMat(const Mat a,const Mat b);
};