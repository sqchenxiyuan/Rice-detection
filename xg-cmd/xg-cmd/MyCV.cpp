#include "stdafx.h"
#include "MyCV.h"

float MyCV::getFeatures_LineLength(const Mat input){
	Size dsize = Size(600, 1000);
	Mat a = Mat(dsize, CV_8UC3);
	resize(input, a, dsize);

	float lineLength = 0;

	cvtColor(a, a, CV_BGR2GRAY);//获取灰度图
	//imshow("提取图-灰度", toshowMat(a));
	GaussianBlur(a, a, Size(31, 31), 1.5, 1.5);
	//imshow("提取图-gauss", toshowMat(b));

	Canny(a, a, 0, 30, 3, 3);
	//imshow("提取图-canny", toshowMat(b));


	vector<vector<Point>> storage;
	vector<Vec4i> hierarchy;
	findContours(a, storage, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < storage.size(); i++)
	{
		RotatedRect minRect = minAreaRect(storage.at(i));
		//drawContours(b, storage, i, Scalar(255, 0, 0), 1, 8);
		//imshow("提取图-边缘", toshowMat(b));
		//if (minRect.angle<-15 && minRect.angle>-30 && minRect.size.width<10){
		float p = minRect.size.height / minRect.size.width;

		if (minRect.angle<-13 && minRect.angle>-33 && p>3){
			rectangle(a, minRect.boundingRect(), Scalar(255, 0, 0));
			drawContours(a, storage, i, Scalar(255, 0, 0), 2, 8);
			vector<Point> points = storage.at(i);
			lineLength += arcLength(points, true) / 4;
		}
	}
	//imshow("提取图-边缘", toshowMat(a));
	//waitKey(0);

	return lineLength;
}

Mat MyCV::toshowMat(const Mat a){

	double fScale = 300.0 / a.cols;//缩放系数  

	//计算目标图像的大小  
	Size dsize = Size(a.cols*fScale, a.rows*fScale);
	Mat b = Mat(dsize, a.type());
	resize(a, b, dsize);

	return b;
}
