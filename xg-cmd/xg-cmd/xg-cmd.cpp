// xg-cmd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

Mat toshowMat(const Mat a);
Mat mutlMat(const Mat a, const Mat b);

int _tmain(int argc, _TCHAR* argv[])
{

	Mat backgroud = imread("src/P1.png");
	//Mat backgroud_show = toshowMat(backgroud);
	cvtColor(backgroud, backgroud, CV_BGR2GRAY);//获取灰度图
	GaussianBlur(backgroud, backgroud, Size(15, 15), 1.5, 1.5);
	Canny(backgroud, backgroud,0, 30, 3);
	imshow("背景图", backgroud);

	Mat test = imread("src/P2.png");
	//Mat test_show = toshowMat(test);
	cvtColor(test, test, CV_BGR2GRAY);//获取灰度图
	GaussianBlur(test, test, Size(15, 15), 1.5, 1.5);
	Canny(test, test, 0, 30, 3);
	imshow("检测图", test);

	//Mat mult = mutlMat(test, backgroud);
	Mat mult = imread("src/P3.png");
	//Mat mult_show = toshowMat(mult);
	imshow("提取图", mult);


	//Mat back;
	//Mat mark = cv::getStructuringElement(2, Size(17,17));
	//cv::morphologyEx(mult, back, MORPH_OPEN, mark);//开运算 先腐蚀后膨胀

	////Mat back_show = toshowMat(back);
	//imshow("开图", back);


	waitKey();
	return 0;
}


Mat toshowMat(const Mat a){

	double fScale = 500.0 / a.cols;//缩放系数  

	//计算目标图像的大小  
	Size dsize = Size(a.cols*fScale, a.rows*fScale);
	Mat b = Mat(dsize, CV_8UC3);
	resize(a, b, dsize);

	return b;
}


Mat mutlMat(const Mat a, const Mat b){
	Size dsize = Size(a.cols, a.rows);
	Mat out = Mat(dsize, CV_8UC3);
	for (int i = 0; i < a.rows&&i < b.rows; i++){
		for (int j = 0; j < a.cols&&i < b.cols; j++){
			Vec3b vec;
			int z = 0;
			for (int q = 0; q < 3; q++){
				int x = a.at<Vec3b>(i, j)[q];
				int y = b.at<Vec3b>(i, j)[q];
				z += abs(x - y);	
			}
			if (z < 50) out.at<Vec3b>(i, j) = Vec3b(0,0,0);
			else out.at<Vec3b>(i, j) = a.at<Vec3b>(i, j);
		}
	}

	return out;

}