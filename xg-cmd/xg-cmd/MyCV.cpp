#include "stdafx.h"
#include "MyCV.h"

float MyCV::getFeatures_LineLength(const Mat input){
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
		
		//if (minRect.angle<-15 && minRect.angle>-30 && minRect.size.width<10){
		float p = minRect.size.height / minRect.size.width;

		if (minRect.angle<-13 && minRect.angle>-33 && p>3){
			//rectangle(a, minRect.boundingRect(), Scalar(255, 0, 0));
			//drawContours(a, storage, i, Scalar(255, 0, 0), 2, 8);
			vector<Point> points = storage.at(i);
			lineLength += arcLength(points, true) / 4;
		}
		
	}
	//imshow("��ȡͼ-��Ե", toshowMat(a));
	//waitKey(0);

	return lineLength;
}

Mat MyCV::toshowMat(const Mat a){

	double fScale = 300.0 / a.cols;//����ϵ��  

	//����Ŀ��ͼ��Ĵ�С  
	Size dsize = Size(a.cols*fScale, a.rows*fScale);
	Mat b = Mat(dsize, a.type());
	resize(a, b, dsize);

	return b;
}

Mat MyCV::rotateImage(const Mat img, int degree){
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
