#pragma once
class CWatcher
{
public:
	CWatcher(string name,Mat background);
	~CWatcher();
	void  shotBackground();
	vector<Mat> shotItem();
	vector<Mat> getBlock(Mat img);
	void start();
private:
	string m_name;
	Mat m_background;
	Mat multMat(const Mat a,const Mat b);
	
	VideoCapture m_cap;
	HANDLE m_handle;
};