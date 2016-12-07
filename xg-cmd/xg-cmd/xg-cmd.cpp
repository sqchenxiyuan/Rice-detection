// xg-cmd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

Mat toshowMat(const Mat a);
Mat mutlMat(const Mat a, const Mat b);
Mat rotateImage1(Mat img, int degree);
vector<Mat> getKuai(Mat img);

int _tmain(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////获取牛奶盒
	//Mat backgroud = imread("src/P1.png");
	////Mat backgroud_show = toshowMat(backgroud);
	////cvtColor(backgroud, backgroud, CV_BGR2GRAY);//获取灰度图
	//GaussianBlur(backgroud, backgroud, Size(15, 15), 1.5, 1.5);
	////Canny(backgroud, backgroud,0, 30, 3);
	//imshow("背景图", backgroud);

	//Mat test = imread("src/P2.png");
	////Mat test_show = toshowMat(test);
	////cvtColor(test, test, CV_BGR2GRAY);//获取灰度图
	//GaussianBlur(test, test, Size(15, 15), 1.5, 1.5);
	////Canny(test, test, 0, 30, 3);
	//imshow("检测图", test);




	//Mat mult = mutlMat(test, backgroud);
	//////////////////////////////////////////////========



	Mat mult = imread("src/P4.png");
	imshow("提取图", toshowMat(mult));
	
	vector<Mat> kuais=getKuai(mult);
	for (int i = 0; i < kuais.size(); i++)
	{
		//imshow("提取图" + i, toshowMat(kuais.at(i)));
		imshow("提取图" + i, toshowMat(getKuai(kuais.at(i)).at(0)));
	}

	waitKey();
	return 0;
}


Mat toshowMat(const Mat a){

	double fScale = 500.0 / a.cols;//缩放系数  

	//计算目标图像的大小  
	Size dsize = Size(a.cols*fScale, a.rows*fScale);
	Mat b = Mat(dsize,a.type());
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
			if (z < 30) out.at<Vec3b>(i, j) = Vec3b(0,0,0);
			else out.at<Vec3b>(i, j) = a.at<Vec3b>(i, j);
		}
	}

	return out;

}


Mat rotateImage1(Mat img, int degree)
{
	degree = -degree;
	double angle = degree  * CV_PI / 180.; // 弧度  
	double a = sin(angle), b = cos(angle);
	int width = img.cols;
	int height = img.rows;
	int width_rotate = int(height * fabs(a) + width * fabs(b));
	int height_rotate = int(width * fabs(a) + height * fabs(b));
	//旋转数组map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float map[6];
	Mat map_matrix = Mat(2, 3, CV_32F, map);
	// 旋转中心
	CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);
	CvMat map_matrix2 = map_matrix;
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);
	map[2] += (width_rotate - width) / 2;
	map[5] += (height_rotate - height) / 2;
	Mat img_rotate;
	//对图像做仿射变换
	//CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。
	//如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.
	//CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，
	warpAffine(img, img_rotate, map_matrix, Size(width_rotate, height_rotate), 1, 0, 0);
	return img_rotate;
}


vector<Mat> getKuai(Mat img){
	vector<Mat> list;
	Mat x,y;
	img.copyTo(x);
	img.copyTo(y);

	cvtColor(x, x, CV_BGR2GRAY);//获取灰度图
	threshold(x,x, 1, 255, CV_THRESH_BINARY);//二值化
	GaussianBlur(x, x, Size(15, 15), 1.5, 1.5);

	//Canny(mult, mult,0, 100, 3);

	//imshow("提取图-canny", toshowMat(mult));

	vector<vector<Point>> storage;
	vector<Vec4i> hierarchy;
	//去除边缘的米   默认为一个米


	cv::findContours(x, storage, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < storage.size(); i++)
	{
		
		RotatedRect minRect = minAreaRect(storage.at(i));
		Rect bo = boundingRect(storage.at(i));
		cout << minRect.angle;

		Mat h = y(bo);
		h = rotateImage1(h, -minRect.angle);
		list.push_back(h);
	}

	for (int i = 0; i < storage.size(); i++)
	{
		drawContours(y, storage, i, Scalar(255, 0, 0), 10, 8);
	}

	imshow("提取图", toshowMat(y));

	return list;
}