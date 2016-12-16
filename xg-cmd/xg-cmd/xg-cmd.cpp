// xg-cmd.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

Mat getNBack();
Mat hunhe(vector<Mat> imgs);


DWORD WINAPI  showViedo(LPVOID lpParam){
	VideoCapture* cap = (VideoCapture*)lpParam;
	Mat now;
	bool stop=false;
	while (!stop)
	{
		*cap >> now;
		imshow("V", now);
		waitKey(30);
	}
	return NULL;
}



int _tmain(int argc, _TCHAR* argv[])
{	
	/*VideoCapture cap(0);
	HANDLE v = CreateThread(0, 0, showViedo, &cap, 0, NULL);

	cout << "=========�����ܼ���=========" << endl;
	CImgSetData false_set("src/false");

	cout << "����ƽ�� : " << false_set.getLength("avg") << endl;
	cout << "���ֵ : " << false_set.getLength("max") << endl;
	cout << "��Сֵ : " << false_set.getLength("min") << endl;

	cout << "=========�����ܼ���=========" << endl;
	CImgSetData true_set("src/true");

	cout << "����ƽ�� : " << true_set.getLength("avg") << endl;
	cout << "���ֵ : " << true_set.getLength("max") << endl;
	cout << "��Сֵ : " << true_set.getLength("min") << endl;*/


	/*Mat bacground;
	cout << "=====¼ȡ����ͼ=====" << endl;
	imshow("����ͼ", Mat(10, 10, CV_8U));
	waitKey(0);
	cap >> bacground;
	imshow("����ͼ", bacground);
	*/





	//////////////////////////////////////////////��ȡţ�̺�
	CWatcher w1(imread("src/test/0.jpg"));

	vector<Mat> kuais = w1.getBlock(imread("src/test/2.jpg"));
	for (int i = 0; i < kuais.size(); i++)
	{
		//imshow("��ȡͼ" + i, toshowMat(kuais.at(i)));
		Mat x = kuais.at(i);
		imshow("��ȡͼ ţ�̺�", x);
		cout << MyCV::getFeatures_LineLength(x)<<"---"<<i << endl;
		waitKey(0);
	}


	//Mat mult = mutlMat(test, backgroud);
	//////////////////////////////////////////////========



	//Mat mult = imread("src/P4.png");
	////imshow("��ȡͼ", toshowMat(mult));
	//Mat x;

	

	///*Size dsize = Size(600, 1000);
	//Mat b = Mat(dsize, x.type());
	//resize(x, b, dsize);*/

	////Mat heback=getNBack();//ţ�̺б���
	//////imshow("ţ�̺б���", toshowMat(heback));
	////cvtColor(heback, heback, CV_BGR2GRAY);//��ȡ�Ҷ�ͼ
	//////imshow("ţ�̺б���-��", toshowMat(heback));
	////Mat back;
	////Mat mark = cv::getStructuringElement(2, Size(51, 51));
	////morphologyEx(heback, heback, MORPH_OPEN, mark);//������ �ȸ�ʴ������
	////imshow("ţ�̺б���-��", toshowMat(heback));



	////b = mutlMat(b, heback);

	////mark = cv::getStructuringElement(2, Size(11,11));
	////morphologyEx(b,b, MORPH_OPEN, mark);
	////imshow("��ȡͼ-��", toshowMat(b));

	//Mat img1 = x;


	//float alllength;
	//float max,min;
	//cout << "=====��ȡ��=====" << endl;
	//cout << MyCV::getFeatures_LineLength(img1) << endl;
	


	
	waitKey(0);
	return 0;
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


bool sizesomp(const vector<Point> &a, const vector<Point> &b){
	RotatedRect x1 = minAreaRect(a);
	RotatedRect x2 = minAreaRect(b);
	return x1.size.area() > x2.size.area();
}