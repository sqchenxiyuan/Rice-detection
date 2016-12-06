#pragma once
class MyCV
{
public:
	MyCV();
	~MyCV();

	static Mat spfz(Mat intimg);
	static Mat Two_valued(Mat in, int center);
	static Mat Iterative_threshold_segmentation(Mat in, float cha);//迭代法阈值分割
	static Mat Iterative_threshold_segmentation(Mat in, float cha,int kuainum);
	static Mat Copy(Mat oldimg, Mat addimg, Point oldpoint, Rect addrect);
	static Mat GetBackgroundimg(Mat in, int sizex, int sizey);
	static Mat Mult(Mat oldimg, Mat multimg);
};

