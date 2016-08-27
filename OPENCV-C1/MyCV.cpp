#include "stdafx.h"
#include "MyCV.h"


MyCV::MyCV()
{
}


MyCV::~MyCV()
{
}

Mat MyCV::spfz(Mat intimg){
	int w, h;
	w = intimg.cols;
	h = intimg.rows;
	Mat out(h, w, CV_8UC3);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			out.at<Vec3b>(i, j) = intimg.at<Vec3b>(i, w - j - 1);
		}
	}
	return out;
}

Mat MyCV::Two_valued(Mat in, int center){
	int w, h;
	w = in.cols;
	h = in.rows;
	Mat out(h, w, CV_8U);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			if (in.at<uchar>(i, j) >= center){
				out.at<uchar>(i, j) = 255;
			}
			else{
				out.at<uchar>(i, j) = 0;
			}
		}
	}
	return out;
}

Mat MyCV::Iterative_threshold_segmentation(Mat in, float cha){
	float high=0,low=0;
	int highnum = 1, lownum = 1;
	int h = in.rows;
	int w = in.cols;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			high += in.at<uchar>(i, j);
		}
	}
	float threshold = high/h/w;//ãÐÖµ
	Mat out(h, w, CV_8U);
	do
	{
		high = 0;
		low = 0;
		highnum = 1;
		lownum = 1;
		for (int i = 0; i < h; i++){
			for (int j = 0; j < w; j++){
				if (in.at<uchar>(i, j) > threshold){
					high += in.at<uchar>(i, j);
					highnum++;
				}
				else{
					low += in.at<uchar>(i, j);
					lownum++;
				}
			}
		}
		if ((high / highnum + low / lownum) / 2 - threshold>cha){
			threshold = (high / highnum + low / lownum) / 2;
		}
		else
		{
			break;
		}
	} while (true);
	//cout << threshold << "<==>" << ((high / highnum + low / lownum) / 2) << endl;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			if (in.at<uchar>(i, j) >= threshold){
				out.at<uchar>(i, j) = 255;
			}
			else{
				out.at<uchar>(i, j) = 0;
			}
		}
	}
	return out;
}

Mat MyCV::Copy(Mat oldimg, Mat addimg, Point oldpoint, Rect addrect){
	Mat out(oldimg);
	int h = oldimg.rows;
	int w = oldimg.cols;
	for (int i = oldpoint.y; i < h && i < oldpoint.y + addrect.height; i++){
		for (int j = oldpoint.x; j < w && j < oldpoint.x + addrect.width; j++){
			if (oldimg.type() == CV_8U){
				out.at<uchar>(i, j) = addimg.at<uchar>(i - oldpoint.y + addrect.y, j - oldpoint.x + addrect.x);
			}
			else{
				out.at<Vec3b>(i, j) = addimg.at<Vec3b>(i - oldpoint.y + addrect.y, j - oldpoint.x + addrect.x);
			}
		}
	}
	return out;
}

Mat MyCV::Iterative_threshold_segmentation(Mat in, float cha, int kuainum){
	if (kuainum % 4 != 0){
		return Iterative_threshold_segmentation(in,cha);
	}
	else{
		Mat e1 = in(Rect(0, 0, in.cols/2, in.rows / 2));
		Mat e2 = in(Rect(0, in.rows / 2, in.cols/2, in.rows - in.rows / 2));
		Mat e3 = in(Rect(in.cols / 2, 0, in.cols-in.cols/2, in.rows / 2));
		Mat e4 = in(Rect(in.cols / 2, in.rows / 2, in.cols - in.cols / 2, in.rows - in.rows / 2));
		e1 = MyCV::Iterative_threshold_segmentation(e1, 0.1,kuainum/4);
		e2 = MyCV::Iterative_threshold_segmentation(e2, 0.1, kuainum / 4);
		e3 = MyCV::Iterative_threshold_segmentation(e3, 0.1, kuainum / 4);
		e4 = MyCV::Iterative_threshold_segmentation(e4, 0.1, kuainum / 4);
		Mat out(in);

		out = MyCV::Copy(out, e1, Point(0, 0), Rect(0, 0, e1.cols, e1.rows));
		out = MyCV::Copy(out, e2, Point(0, in.rows / 2), Rect(0, 0, e2.cols, e2.rows));
		out = MyCV::Copy(out, e3, Point(in.cols / 2, 0), Rect(0, 0, e3.cols, e3.rows));
		out = MyCV::Copy(out, e4, Point(in.cols / 2, in.rows / 2), Rect(0, 0, e4.cols, e4.rows));

		return out;
	}
}

Mat MyCV::GetBackgroundimg(Mat in, int sizex, int sizey){
	int w, h;
	w = in.cols;
	h = in.rows;
	if (sizex % 2 != 1&&sizey%2!=1) return Mat(h, w, CV_8U, Scalar(0));
	Mat out(h, w, CV_8U);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			//int sum=0;
			int min = 255;
			for (int x1 = -sizex/2; x1 <= sizex/2; x1++){
				for (int x2 = -sizey/2; x2 <= sizey/2; x2++){
					if (i + x1<0 || i + x1>=h || j + x2<0 || j + x2>=w) {
						//sum += in.at<uchar>(i, j);
						continue;
					}
					else{
						//sum += in.at<uchar>(i + x1, j + x2);
						if (min > in.at<uchar>(i + x1, j + x2)){
							min = in.at<uchar>(i + x1, j + x2);
						}
					}
				}
			}
			//cout << sum / sizex / sizey << "====" << in.at<uchar>(i, j) << endl;
			out.at<uchar>(i, j) = min;
		}
	}
	return out;
}

Mat MyCV::Mult(Mat oldimg, Mat multimg){
	int w, h;
	w = oldimg.cols;
	h = oldimg.rows;
	Mat out(h, w, CV_8U);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			int x = oldimg.at<uchar>(i, j) - multimg.at<uchar>(i, j);
			if (x < 0) x = 0;
			out.at<uchar>(i, j) = x;
		}
	}
	return out;
}