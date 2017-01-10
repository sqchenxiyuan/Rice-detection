#include "stdafx.h"
#include "ImgSetData.h"


CImgSetData::CImgSetData(){
	
}

CImgSetData::CImgSetData(string dirpath)
{
	m_dirpath = dirpath;
	cout << "=====加载集合数据中=====" << endl;
	loadImages();
	cout << "=====分析集合数据中=====" << endl;
	getFeatures();
}


CImgSetData::~CImgSetData()
{
}


void CImgSetData::addTrans(vector<Mat> imgs){

	for (int i = 0; i < imgs.size(); i++){
		Mat x = imgs.at(i);
		m_imgs.push_back(x);
		int l = MyCV::getFeatures_LineLength(x);

		length_all += l;
		length_avg = length_all / m_imgs.size();
		if (l > length_max)length_max = l;
		if (l < length_min)length_min = l;

		cout << "max:" << length_max << " avg:" << length_avg << " min:" << length_min << " imgcount:" << m_imgs.size() << endl;
	}

	
}

float CImgSetData::getLength(string type){
	if (type == "all"){
		return length_all;
	}
	else if (type == "avg"){
		return length_avg;
	}
	else if (type == "max"){
		return length_max;
	}
	else if (type == "min"){
		return length_min;
	}
	return 0;
}

float CImgSetData::getAcquaintance(const Mat img){
	float Features_length = MyCV::getFeatures_LineLength(img);
	
	/*if (Features_length > length_avg - (length_avg - length_min) *0.8&&
		Features_length < length_avg + (length_max - length_avg) *0.8){
		return 1;
	}
	else if (Features_length < length_avg - (length_avg - length_min) *0.8){
		return Features_length / (length_avg - (length_avg - length_min) *0.8);
	}
	else if (Features_length > length_avg + (length_max - length_avg) *0.8){
		return Features_length / (length_avg + (length_max - length_avg) *0.8) - 1;
	}*/

	//cout << Features_length << endl;
	/*if (Features_length > 2 * length_min - length_avg&&
		Features_length < 2 * length_avg - length_max){
		return 1;
	}
	else{
		return 0;
	}*/

	float lmin = length_avg - length_min;
	float lmax = length_max - length_avg;
	float c = lmax + lmin - abs(Features_length - length_avg);
	if (c < 0)c = 0;
	float out = c / (lmax + lmin);
	return out;

	/*if (Features_length > length_min/2&&
		Features_length < length_max*3/2){
		return 1;
	}
	else{
		return 0;
	}*/
}

void CImgSetData::loadImages(){
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(m_dirpath).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib &  _A_SUBDIR))
			{
				Mat x = imread(p.assign(m_dirpath).append("\\").append(fileinfo.name));
				m_imgs.push_back(x);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void CImgSetData::getFeatures(){

	length_all = 0;
	length_min = FLT_MAX;
	length_max = 0;
	for (int i = 0; i < m_imgs.size(); i++){
		float l = MyCV::getFeatures_LineLength(m_imgs.at(i));
		/*cout << "图片 " << i + 1 << " : " << l << endl;*/
		length_all += l;
		if (l > length_max)length_max = l;
		else if (l < length_min)length_min = l;
	}
	length_avg = length_all / m_imgs.size();
}