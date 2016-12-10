// xg-cmd.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

Mat toshowMat(const Mat a);
Mat mutlMat(const Mat a, const Mat b);
Mat rotateImage1(Mat img, int degree);
Mat getNBack();
Mat hunhe(vector<Mat> imgs);
vector<Mat> getKuai(Mat img);
Mat mutlMat_gray(const Mat a, const Mat b);


//��ȡţ�̺з�������ֵ
float getFeatures_LineLength(const Mat input);


int _tmain(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////��ȡţ�̺�
	//Mat backgroud = imread("src/P1.png");
	////Mat backgroud_show = toshowMat(backgroud);
	////cvtColor(backgroud, backgroud, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	//GaussianBlur(backgroud, backgroud, Size(15, 15), 1.5, 1.5);
	////Canny(backgroud, backgroud,0, 30, 3);
	//imshow("����ͼ", backgroud);

	//Mat test = imread("src/P2.png");
	////Mat test_show = toshowMat(test);
	////cvtColor(test, test, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	//GaussianBlur(test, test, Size(15, 15), 1.5, 1.5);
	////Canny(test, test, 0, 30, 3);
	//imshow("���ͼ", test);




	//Mat mult = mutlMat(test, backgroud);
	//////////////////////////////////////////////========



	Mat mult = imread("src/P4.png");
	//imshow("��ȡͼ", toshowMat(mult));
	Mat x;

	vector<Mat> kuais=getKuai(mult);
	for (int i = 0; i < kuais.size(); i++)
	{
		//imshow("��ȡͼ" + i, toshowMat(kuais.at(i)));
		x = getKuai(kuais.at(i)).at(i);
		//imshow("��ȡͼ ţ�̺�", toshowMat(x));
	}

	/*Size dsize = Size(600, 1000);
	Mat b = Mat(dsize, x.type());
	resize(x, b, dsize);*/

	//Mat heback=getNBack();//ţ�̺б���
	////imshow("ţ�̺б���", toshowMat(heback));
	//cvtColor(heback, heback, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	////imshow("ţ�̺б���-��", toshowMat(heback));
	//Mat back;
	//Mat mark = cv::getStructuringElement(2, Size(51, 51));
	//morphologyEx(heback, heback, MORPH_OPEN, mark);//������ �ȸ�ʴ������
	//imshow("ţ�̺б���-��", toshowMat(heback));



	//b = mutlMat(b, heback);

	//mark = cv::getStructuringElement(2, Size(11,11));
	//morphologyEx(b,b, MORPH_OPEN, mark);
	//imshow("��ȡͼ-��", toshowMat(b));

	Mat img1 = x;


	float alllength;
	float max,min;
	cout << "=====��ȡ��=====" << endl;
	cout << getFeatures_LineLength(img1) << endl;



	cout << "=====�����ܼ���=====" << endl;
	vector<Mat> falseset;
	falseset.push_back(imread("src/false/1.jpg"));
	falseset.push_back(imread("src/false/2.jpg"));
	falseset.push_back(imread("src/false/3.jpg"));
	falseset.push_back(imread("src/false/4.jpg"));
	falseset.push_back(imread("src/false/5.jpg"));

	alllength = 0;
	min = FLT_MAX;
	max = 0;
	for (int i = 0; i < falseset.size(); i++){
		float l = getFeatures_LineLength(falseset.at(i));
		cout << "ͼƬ " << i + 1 << " : " << l << endl;
		alllength += l;
		if (l > max)max = l;
		else if (l < min)min = l;
	}
	cout << "����ƽ�� : " << alllength / falseset.size() << endl;
	cout << "���ֵ : " << max << endl;
	cout << "��Сֵ : " << min << endl;


	cout << "=====�����ܼ���=====" << endl;


	vector<Mat> trueset;
	trueset.push_back(imread("src/true/img1.jpg"));
	trueset.push_back(imread("src/true/img2.jpg"));
	trueset.push_back(imread("src/true/img3.jpg"));
	trueset.push_back(imread("src/true/img4.jpg"));
	trueset.push_back(imread("src/true/img5.jpg"));

	alllength = 0;
	min = FLT_MAX;
	max = 0;
	for (int i = 0; i < trueset.size(); i++){
		float l = getFeatures_LineLength(trueset.at(i));
		cout << "ͼƬ " << i + 1 << " : " << l << endl;
		alllength += l;
		if (l > max)max = l;
		else if (l < min)min = l;
	}
	cout << "��ȷƽ�� : " << alllength / trueset.size() << endl;
	cout << "���ֵ : " << max << endl;
	cout << "��Сֵ : " << min << endl;


	waitKey();
	return 0;
}


Mat toshowMat(const Mat a){

	double fScale = 500.0 / a.cols;//����ϵ��  

	//����Ŀ��ͼ��Ĵ�С  
	Size dsize = Size(a.cols*fScale, a.rows*fScale);
	Mat b = Mat(dsize,a.type());
	resize(a, b, dsize);

	return b;
}


Mat mutlMat(const Mat a, const Mat b){
	Size dsize = Size(a.cols, a.rows);
	Mat out = Mat(dsize, CV_8UC3);
	for (int i = 0; i < a.rows&&i < b.rows; i++){
		for (int j = 0; j < a.cols&&j < b.cols; j++){
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

Mat mutlMat_gray(const Mat a, const Mat b){
	Size dsize = Size(a.cols, a.rows);
	Mat out = Mat(dsize,CV_8U);
	for (int i = 0; i < a.rows&&i < b.rows; i++){
		for (int j = 0; j < a.cols&&j < b.cols; j++){
			int x = a.at<uchar>(i, j);
			int y = b.at<uchar>(i, j);
			int z = abs(x - y);
			//if (z < 30) out.at<uchar>(i, j) = 0;
			//else out.at<uchar>(i, j) = z;
			out.at<uchar>(i, j) = z;
		}
	}
	return out;
}


Mat rotateImage1(Mat img, int degree)
{
	degree = -degree;
	double angle = degree  * CV_PI / 180.; // ����  
	double a = sin(angle), b = cos(angle);
	int width = img.cols;
	int height = img.rows;
	int width_rotate = int(height * fabs(a) + width * fabs(b));
	int height_rotate = int(width * fabs(a) + height * fabs(b));
	//��ת����map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float map[6];
	Mat map_matrix = Mat(2, 3, CV_32F, map);
	// ��ת����
	CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);
	CvMat map_matrix2 = map_matrix;
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);
	map[2] += (width_rotate - width) / 2;
	map[5] += (height_rotate - height) / 2;
	Mat img_rotate;
	//��ͼ��������任
	//CV_WARP_FILL_OUTLIERS - ����������ͼ������ء�
	//�������������������ͼ��ı߽��⣬��ô���ǵ�ֵ�趨Ϊ fillval.
	//CV_WARP_INVERSE_MAP - ָ�� map_matrix �����ͼ������ͼ��ķ��任��
	warpAffine(img, img_rotate, map_matrix, Size(width_rotate, height_rotate), 1, 0, 0);
	return img_rotate;
}

Mat getNBack(){
	Mat i1 = imread("src/back/1.jpg");
	Mat i2 = imread("src/back/2.jpg");
	Mat i3 = imread("src/back/3.jpg");
	Mat i4 = imread("src/back/4.jpg");
	Mat i5 = imread("src/back/5.jpg");

	vector<Mat> imgs;
	imgs.push_back(i1);
	imgs.push_back(i2);
	imgs.push_back(i3);
	imgs.push_back(i4);
	imgs.push_back(i5);

	return  hunhe(imgs);
}

Mat hunhe(vector<Mat> imgs){
	Size dsize = Size(600, 1000);
	Mat out = Mat(dsize, CV_8UC3);

	for (int x = 0; x < imgs.size(); x++){
		Mat a = imgs.at(x);
		Mat b = Mat(dsize, a.type());
		resize(a, b, dsize);
		imgs.at(x) = b;
		
	}

	for (int i = 0;i < out.rows; i++){
		for (int j = 0; j < out.cols; j++){
			int x0=0, x1=0, x2=0;

			for (int x = 0; x < imgs.size(); x++){
				Mat a = imgs.at(x);
				x0 += a.at<Vec3b>(i, j)[0];
				x1 += a.at<Vec3b>(i, j)[1];
				x2 += a.at<Vec3b>(i, j)[2];
			}
			x0 /= imgs.size();
			x1 /= imgs.size();
			x2 /= imgs.size();

			Vec3b vec(x0, x1, x2);
			out.at<Vec3b>(i, j) = vec;
			//cout << x0 << "," << x1 << "," << x2 << endl;
			//cout << out.at<Vec3b>(i, j)[0] << "," << out.at<Vec3b>(i, j)[1] << "," << out.at<Vec3b>(i, j)[2] << endl;

		}
	}
	GaussianBlur(out, out, Size(15, 15), 1.5, 1.5);

	//imshow("test",toshowMat(out));
	return out;
}

vector<Mat> getKuai(Mat img){
	vector<Mat> list;
	Mat x,y;
	img.copyTo(x);
	img.copyTo(y);

	cvtColor(x, x, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	threshold(x,x, 1, 255, CV_THRESH_BINARY);//��ֵ��
	GaussianBlur(x, x, Size(5, 5), 1, 1);

	//Canny(mult, mult,0, 100, 3);

	//imshow("��ȡͼ-canny", toshowMat(mult));

	vector<vector<Point>> storage;
	vector<Vec4i> hierarchy;
	//ȥ����Ե����   Ĭ��Ϊһ����

	/*imshow("test1",x);*/
	cv::findContours(x, storage, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < storage.size(); i++)
	{
		
		RotatedRect minRect = minAreaRect(storage.at(i));
		Rect bo = boundingRect(storage.at(i));
		//cout << minRect.angle;
		//imshow("test", y);

		Mat h = y(bo).clone();
		h = rotateImage1(h, -minRect.angle);
		
		list.push_back(h);

		rectangle(y, bo, Scalar(0, 0, 255));
	}

	for (int i = 0; i < storage.size(); i++)
	{
		drawContours(y, storage, i, Scalar(255, 0, 0), 2, 8);
	}

	//imshow("��ȡͼ123", toshowMat(y));

	return list;
}




float getFeatures_LineLength(const Mat input){
	Size dsize = Size(600, 1000);
	Mat a = Mat(dsize, CV_8UC3);
	resize(input, a, dsize);

	float lineLength = 0;

	cvtColor(a, a, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	//imshow("��ȡͼ-�Ҷ�", toshowMat(a));
	GaussianBlur(a, a, Size(31, 31), 1.5, 1.5);
	//imshow("��ȡͼ-gauss", toshowMat(b));

	Canny(a, a, 0, 30, 3, 3);
	//imshow("��ȡͼ-canny", toshowMat(b));


	vector<vector<Point>> storage;
	vector<Vec4i> hierarchy;
	findContours(a, storage, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	
	for (int i = 0; i < storage.size(); i++)
	{
		RotatedRect minRect = minAreaRect(storage.at(i));
		//drawContours(b, storage, i, Scalar(255, 0, 0), 1, 8);
		//imshow("��ȡͼ-��Ե", toshowMat(b));
		//if (minRect.angle<-15 && minRect.angle>-30 && minRect.size.width<10){
		float p = minRect.size.height / minRect.size.width;

		if (minRect.angle<-15 && minRect.angle>-30 && p>3){
			rectangle(a, minRect.boundingRect(), Scalar(255, 0, 0));
			drawContours(a, storage, i, Scalar(255, 0, 0), 2, 8);
			vector<Point> points = storage.at(i);
			lineLength += arcLength(points, true) / 4;
		}
	}
	imshow("��ȡͼ-��Ե", toshowMat(a));
	waitKey(0);

	return lineLength;
}