#pragma once
class CImgSetData
{
public:
	CImgSetData();
	CImgSetData(string dirpath);
	~CImgSetData();

	float getLength(string type);
	float getAcquaintance(const Mat img);

	void addTrans(vector<Mat> imgs);
private:
	string m_dirpath;
	vector<Mat> m_imgs;
	void loadImages();
	void getFeatures();

	float length_all=0;
	float length_max=0;
	float length_avg=0;
	float length_min = FLT_MAX;
};