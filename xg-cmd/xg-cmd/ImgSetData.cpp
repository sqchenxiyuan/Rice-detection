#include "stdafx.h"
#include "ImgSetData.h"


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