// xg-cmd.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

Mat toshowMat(const Mat a);
Mat mutlMat(const Mat a, const Mat b);

int _tmain(int argc, _TCHAR* argv[])
{

	Mat backgroud = imread("src/P1.png");
	//Mat backgroud_show = toshowMat(backgroud);
	cvtColor(backgroud, backgroud, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	GaussianBlur(backgroud, backgroud, Size(15, 15), 1.5, 1.5);
	Canny(backgroud, backgroud,0, 30, 3);
	imshow("����ͼ", backgroud);

	Mat test = imread("src/P2.png");
	//Mat test_show = toshowMat(test);
	cvtColor(test, test, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	GaussianBlur(test, test, Size(15, 15), 1.5, 1.5);
	Canny(test, test, 0, 30, 3);
	imshow("���ͼ", test);

	//Mat mult = mutlMat(test, backgroud);
	Mat mult = imread("src/P3.png");
	//Mat mult_show = toshowMat(mult);
	imshow("��ȡͼ", mult);


	//Mat back;
	//Mat mark = cv::getStructuringElement(2, Size(17,17));
	//cv::morphologyEx(mult, back, MORPH_OPEN, mark);//������ �ȸ�ʴ������

	////Mat back_show = toshowMat(back);
	//imshow("��ͼ", back);


	waitKey();
	return 0;
}


Mat toshowMat(const Mat a){

	double fScale = 500.0 / a.cols;//����ϵ��  

	//����Ŀ��ͼ��Ĵ�С  
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