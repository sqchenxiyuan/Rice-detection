#include "stdafx.h"
#include "Watcher.h"

DWORD WINAPI showVideo(LPVOID lpParam){
	VideoCapture* cap = (VideoCapture*)lpParam;
	Mat now;
	bool stop = false;
	string name = "Wacter" + int(rand() * 10000 / RAND_MAX + 1.0);
	while (!stop)
	{
		*cap >> now;
		imshow(name, now);
		if (waitKey(30) == 113){
			stop = true;
		}
	}
	return NULL;
}

CWatcher::CWatcher(string name,Mat background)
{
	m_name = name;
	cout << "加载观察员中" << endl;
	m_cap=VideoCapture(0);
	GaussianBlur(background, background, Size(7, 7), 1.5, 1.5);
	m_background = background;
	m_handle = CreateThread(0, 0, showVideo, &m_cap, 0, NULL);
	cout << "加载观察员加载完成" << endl;
	//system("Cls");
}


CWatcher::~CWatcher()
{
}


bool sizecomp(const vector<Point> &a, const vector<Point> &b){
	RotatedRect x1 = minAreaRect(a);
	RotatedRect x2 = minAreaRect(b);
	return x1.size.area() > x2.size.area();
}

void CWatcher::shotBackground(){
	Mat x;
	m_cap >> x;
	GaussianBlur(x,x , Size(7, 7), 1.5, 1.5);
	m_background = x;
	imshow("背景图", m_background);
}

vector<Mat> CWatcher::shotItem(){
	Mat shot;
	m_cap >> shot;
	return getBlock(shot);
}

vector<Mat> CWatcher::getBlock(Mat img){
	GaussianBlur(img, img, Size(7, 7), 1.5, 1.5);

	Mat mult = multMat(img, m_background);

	Mat mark = cv::getStructuringElement(2, Size(3, 3));
	morphologyEx(mult, mult, MORPH_OPEN, mark);//开运算 先腐蚀后膨胀

	vector<Mat> blocks;

	cvtColor(mult, mult, CV_BGR2GRAY);//获取灰度图
	threshold(mult, mult, 1, 255, CV_THRESH_BINARY);//二值化
	GaussianBlur(mult, mult, Size(5, 5), 1, 1);



	vector<vector<Point>> storage;
	vector<Vec4i> hierarchy;
	cv::findContours(mult, storage, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	/*cout << storage.size() << endl;*/
	sort(storage.begin(), storage.end(), sizecomp);

	vector<vector<Point>> boxs;
	for (int i = 0; i < storage.size(); i++)
	{
		vector<Point> newkuai = storage.at(i);
		RotatedRect minRect = minAreaRect(newkuai);
		bool in = false;
		for (int j = 0; j < boxs.size(); j++){
			Rect bo = boundingRect(boxs.at(j));
			float WLRatio = minRect.size.width / minRect.size.height;
			if (WLRatio<0.5 ||
				WLRatio>0.7 ||
				minRect.size.area()<1000||
				(bo.x<minRect.center.x&&
				bo.y<minRect.center.y&&
				bo.x + bo.width>minRect.center.x&&
				bo.y + bo.height>minRect.center.y)){
				in = true;
				break;
			}
		}
		if (!in){
			boxs.push_back(newkuai);
		}
	}

	for (int i = 0; i < boxs.size(); i++)
	{

		RotatedRect minRect = minAreaRect(boxs.at(i));
		Rect bo = boundingRect(boxs.at(i));

		Mat h = img(bo).clone();
		h = MyCV::rotateImage(h, -minRect.angle);
		float height = minRect.size.height;
		float width = minRect.size.width;
		Rect rect(h.cols/2-width/2, h.rows/2-height/2, width, height);
		Mat out = h(rect).clone();
		if (MyCV::getFeatures_LineLength(out) > 10){
			blocks.push_back(out);
		}

		/*rectangle(img, bo, Scalar(0, 0, 255));
		drawContours(img, storage, i, Scalar(255, 0, 0), 2, 8);
		imshow("test", img);
		waitKey(0);*/
	}

	return blocks;
}


Mat CWatcher::multMat(const Mat a, const Mat b){
	Size dsize = Size(a.cols, a.rows);
	Mat out = Mat(dsize, CV_8UC3);
	for (int i = 0; i < a.rows&&i < b.rows; i++){
		for (int j = 0; j < a.cols&&j < b.cols; j++){
			Vec3b vec;
			int z = 0;
			for (int q = 0; q < 3; q++){
				int x = a.at<Vec3b>(i, j)[q];
				int y = b.at<Vec3b>(i, j)[q];
				z += x - y;
			}
			if (z < 50) out.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			else out.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			//else out.at<Vec3b>(i, j) = a.at<Vec3b>(i, j);
		}
	}
	return out;
}

