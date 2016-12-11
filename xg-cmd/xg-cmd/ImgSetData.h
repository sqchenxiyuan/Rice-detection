#pragma once
class CImgSetData
{
public:
	CImgSetData(string dirpath);
	~CImgSetData();

	float getLength(string type);

private:
	string m_dirpath;
	vector<Mat> m_imgs;
	void loadImages();
	void getFeatures();

	float length_all;
	float length_max;
	float length_avg;
	float length_min;
};