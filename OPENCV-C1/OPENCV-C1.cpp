// OPENCV-C1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MyCV.h"

#define  PI 3.1415926

struct Rice{
	bool abandoned = false;
	bool edge = false;
	bool link = false;
	bool incomplete = false;
	int ricenum=1;//预估米粒数
	float circumference;//周长
	float areasize;//面积
	float roundness;//圆形度
	float rectangularity;//矩形度
	float minRectsize;//最小外接圆面积
	float minRectlengthwidthratio;//最小外接圆长宽比
	vector<Point> minRectpoints;
	vector<Point> points;//边缘
	RotatedRect minRect;//最小外接矩形
};

struct allRice{
	int areanum = 0;
	int edgenum = 0;
	int linknum = 0;
	int incompletenum = 0;
	int ricenum=0;//预估米粒数
	float circumference = 0;//平均周长
	float areasize = 0;//平均面积
	float roundness = 0;//圆形度
	float rectangularity = 0;//矩形度
	float lengthwidthratio = 0;//长宽比
};

allRice getallRicedata(vector<Rice> rices);
Mat drawRect(Mat input, vector<Point> points);
int _tmain(int argc, _TCHAR* argv[])
{
	
	Mat frame;
	Mat edges;
	bool stop = false;
	frame = imread("1.jpg");
	/*Mat edges(frame.rows+2,frame.cols+2,frame.type(),Scalar(0));
	frame = MyCV::Copy(edges, frame, Point(1, 1), Rect(0, 0, frame.cols, frame.rows));*/
	//imshow("原图", frame);
	cvtColor(frame, edges, CV_BGR2GRAY);//获取灰度图

	GaussianBlur(edges, edges, Size(3, 3), 1.5, 1.5);//高斯模糊
	//获取背景
	Mat back;
	Mat mark = cv::getStructuringElement(2,Size(51,51));
	cv::morphologyEx(edges, back, MORPH_OPEN, mark);//开运算 先腐蚀后膨胀
	/*cv::erode(edges, back, mark);
	cv::dilate(back, back, mark);*/

	//imshow("背景", back);
	
	edges = MyCV::Mult(edges, back);//去背景
	/////////////////////////////////////////////////////////二值化
	edges = MyCV::Iterative_threshold_segmentation(edges, 0.1);


	Mat mark1 = cv::getStructuringElement(2, Size(7, 7));
	Mat mark2 = cv::getStructuringElement(2, Size(7, 7));
	cv::erode(edges, edges, mark1);
	cv::dilate(edges, edges, mark2);
	imshow("二值化图", edges);

	/////////////////////////////////////////////////////////边缘检测


	////imshow("当前视频2", edges);
	//GaussianBlur(edges, edges, Size(15, 15), 1.5, 1.5);
	////imshow("当前视频3", edges);
	//Canny(edges, edges, 0, 30, 3);
	//imshow("边缘", edges);
	Mat out=frame.clone();
	vector<vector<Point>> storage;
	vector<Vec4i> hierarchy;
	vector<Rice> rices;

	//去除边缘的米   默认为一个米


	cv::findContours(edges, storage, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < storage.size(); i++)
	{
		Rice newRice;
		//cv::drawContours(out, storage, i, Scalar(255, 0, 0), 1, 8);
		//获取面积
		float areasize = fabs(cv::contourArea(storage.at(i)));
		//获取周长
		vector<Point> points = storage.at(i);
		float circumference = arcLength(points,true);

		//获取最小外接矩形
		RotatedRect minRect = minAreaRect(storage.at(i));

		//最小外接矩形长宽比
		float lengthwidthratio = minRect.size.height / minRect.size.width;
		//if (minRect.size.height == 0 || minRect.size.width == 0){
		//	//cv::drawContours(frame, storage, i, Scalar(0, 0, 255), 3, 8);
		/*	Point2f point[4];
			minRect.points(point);
			out=drawRect(out, point);*/
		//	minRect.points(point);
		//	cout << point[0]<< endl;
		//	cout << point[1] << endl;
		//	cout << point[2] << endl;
		//	cout << point[3] << endl;
		//	cout << minRect.size.height << "====" << minRect.size.width << endl;
		//}
		if (lengthwidthratio < 1)lengthwidthratio = 1 / lengthwidthratio;
		if (minRect.size.height == 0 || minRect.size.width == 0){
			newRice.abandoned = true;
		}
		//contour_area_sum += contour_area_tmp; //求所有轮廓的面积和

		//
		Point2f point[4];
		vector<Point> minRectpoints;
		minRect.points(point);
		for (int j = 0; j < 4; j++)
		{
			minRectpoints.push_back(point[j]);
		}
		newRice.points = storage.at(i);
		newRice.minRectpoints = minRectpoints;
		newRice.areasize = areasize;
		newRice.circumference = circumference;
		newRice.minRect = minRect;
		newRice.minRectsize = minRect.size.height*minRect.size.width;
		newRice.minRectlengthwidthratio = lengthwidthratio;
		newRice.roundness = 4 * PI*areasize / circumference / circumference;
		newRice.rectangularity = areasize / newRice.minRectsize;

		rices.push_back(newRice);
		//waitKey(30);
	}
	imshow("OUT", out);
	waitKey();
	allRice allrice = getallRicedata(rices);
	cout <<	"////////////////初始////////////////" << endl;
	cout << "区域总数" << allrice.areanum << endl;
	cout << "预估米粒总数" << allrice .ricenum<< endl;
	cout << "总圆形度" << allrice.roundness<< endl;
	cout << "总矩形度" << allrice.rectangularity << endl;
	cout << "总长宽比" << allrice.lengthwidthratio << endl;
	cout << "平均面积" << allrice.areasize << endl;

	for (int i = 0;;){

		bool change = false;
		Mat out = frame.clone();

		for (int j = 0; j < rices.size(); j++)
		{
			Rice *rice = &rices.at(j);
			/*vector<Point> points = rices.at(j).minRectpoints;
			out = drawRect(out, points);*/
			if (!rices.at(j).abandoned){
				vector<Point> minRectpoints = rices.at(j).minRectpoints;
				out = drawRect(out, minRectpoints);
			
			
				//初始值
				float minsize = allrice.areasize*0.8;
				float maxsize = allrice.areasize*1.5;

				float minlengthwidthratio = allrice.lengthwidthratio*0.8;
				float maxlengthwidthratio = allrice.lengthwidthratio*2;

				float minroundness = allrice.roundness*0.9;
				float maxroundness = allrice.roundness*1.2;




				if (i == 0){
					//去除边缘的米   默认为一个米
					vector<Point> points = rice->points;
					for (int x = 0; x < points.size(); x++)
					{
						Point point = points.at(x);
						
						if (point.x<2 || point.x>out.cols - 3 || point.y<2 || point.y>out.rows - 3){
							cv::drawContours(out, storage, j, Scalar(0, 255, 0), 1, 8);
							if (rice->areasize < minsize){
								cv::drawContours(out, storage, j, Scalar(255, 0, 0), 1, 8);
								rice->abandoned = true;
								change = true;
							}
							rice->edge = true;
							break;
						}
					}
				}
				else if(i==1){
					//检查残米
					if (rices.at(j).areasize < minsize&&!rice->abandoned){
						cv::drawContours(out, storage, j, Scalar(255, 0, 0), 1, 8);
						/*cout << rices.at(j).minRectpoints.at(0).x << "==0==" << rices.at(j).minRectpoints.at(0).y << endl;
						cout << rices.at(j).minRectpoints.at(1).x << "==1==" << rices.at(j).minRectpoints.at(1).y << endl;
						cout << rices.at(j).minRectpoints.at(2).x << "==2==" << rices.at(j).minRectpoints.at(2).y << endl;
						cout << rices.at(j).minRectpoints.at(3).x << "==3==" << rices.at(j).minRectpoints.at(3).y << endl;*/
						//cout << rices.at(j).minRectlengthwidthratio << "====" << allrice.lengthwidthratio << endl;
						if ((rices.at(j).minRectlengthwidthratio<minlengthwidthratio || 
							rices.at(j).minRectlengthwidthratio>maxlengthwidthratio)){
							cv::drawContours(out, storage, j, Scalar(0, 0, 255), 1, 8);
							rice->abandoned = true;
							rice->incomplete = true;
							change = true;
						}
					}
					/*if ((rices.at(j).minRectlengthwidthratio<minlengthwidthratio || rices.at(j).minRectlengthwidthratio>maxlengthwidthratio) && !rice->abandoned){
						cv::drawContours(out, storage, j, Scalar(0, 255, 0), 1, 8);
						rices.at(j).abandoned = true;
						change = true;
					}*/
				}
				else if (i == 2){
					if (rice->areasize < maxsize && (rices.at(j).minRectlengthwidthratio<minlengthwidthratio
						|| rices.at(j).minRectlengthwidthratio>maxlengthwidthratio)
						&& !rice->edge){
						cv::drawContours(out, storage, j, Scalar(255, 0, 255), 1, 8);
						rice->abandoned = true;
						rice->ricenum = 2;
						rice->link = true;
						rice->incomplete = true;
						change = true;
					}
				}
				else{
					//检测多米共存
					if (rice->areasize > maxsize&&!rice->abandoned){
						cv::drawContours(out, storage, j, Scalar(255, 255, 0), 1, 8);
						cout << rice->areasize << "====" << allrice.areasize << "====" << rice->areasize / allrice.areasize << endl;
						int sum = rice->areasize / allrice.areasize;
						if (sum + 1 - rice->areasize / allrice.areasize < 0.5){
							sum++;
						}
						rice->abandoned = true;
						rice->ricenum = sum;
						rice->link = true;
						change = true;
					}
				}



				//cout << rices.at(i).areasize << "===" << allrice.areasize << endl;
				//cv::drawContours(out, storage, j, Scalar(255, 0, 0), 1, 8);
				//cout << rices.at(j).minRectlengthwidthratio << "===" << allrice.lengthwidthratio << endl;
				imshow("OUT", out);
				//waitKey(30);
			
			}
		}

		imshow("OUT", out);

		if (change){
			allrice = getallRicedata(rices);
			cout << "////////////////第" << i << "次////////////////" << endl;
			cout << "区域总数" << allrice.areanum << endl;
			cout << "预估米粒总数" << allrice.ricenum << endl;
			cout << "总圆形度" << allrice.roundness << endl;
			cout << "总矩形度" << allrice.rectangularity << endl;
			cout << "总长宽比" << allrice.lengthwidthratio << endl;
			cout << "平均面积" << allrice.areasize << endl;
		}
		else{
			if (i < 3){
				cout << "////////////////第" << i << "次完成////////////////" << endl;
				i++;
			}
			else
			{
				cout << "////////////////无变化，检查结束////////////////" << endl;
				break;
			}
		}
		waitKey();

	}

	allrice = getallRicedata(rices);
	cout << "////////////////结果////////////////" << endl;
	
	out = frame.clone();

	for (int i = 0; i < rices.size(); i++)
	{
		Rice *rice = &rices.at(i);
		int r =0, g =0, b = 0;
		string str = "";
		if (rice->edge) {
			str += " 这块区域沾边 ||";
			b = 1;
		}
		if (rice->link){
			str += " 这块区域可能有" + int(rice->ricenum - 1);
			str += "到" + int(rice->ricenum);
			str += "个米粘连 ||";
			g = 1;
		}
		if (rice->incomplete) {
			str += "这块区域可能包含残缺的米";
			r = 1;
		}
		cv::drawContours(out, storage, i, Scalar(b*255, g*255, r*255), 1, 8);
		if (str != ""){
			cout << "区域" << i << "=》 估计米粒数:" << rice->ricenum << " 圆形度:" << rice->roundness
				<< " 矩形度:" << rice->rectangularity << " 长宽比:" << rice->minRectlengthwidthratio
				<< " 面积:" << rice->areasize << endl;
			cout << "备注:" << str << endl;
		}
	}

	cout << "////////////////总和////////////////" << endl;
	cout << "区域总数" << allrice.areanum << endl;
	cout << "预估米粒总数" << allrice.ricenum << endl;
	cout << "边缘米数量" << allrice.edgenum << endl;
	cout << "粘连米数量" << allrice.linknum << endl;
	cout << "残缺米数量" << allrice.incompletenum << endl;
	cout << "总圆形度" << allrice.roundness << endl;
	cout << "总矩形度" << allrice.rectangularity << endl;
	cout << "总长宽比" << allrice.lengthwidthratio << endl;
	cout << "平均面积" << allrice.areasize << endl;
	imshow("OUT", out);

	//imshow("OUT", frame);

	//VideoCapture cap(0);
	//if (!cap.isOpened())
	//{
	//return -1;
	//}
	//while (!stop)
	//{
	//	cap >> frame;
	//	frame = MyCV::spfz(frame);
	//	//imshow("当前视频1", frame);
	//	cvtColor(frame, edges, CV_BGR2GRAY);
	//	//imshow("当前视频2", edges);
	//	GaussianBlur(edges, edges, Size(15, 15), 1.5, 1.5);
	//	//imshow("当前视频3", edges);
	//	edges = MyCV::Iterative_threshold_segmentation(edges, 1);
	//	//Canny(edges, edges, 0, 30, 3);
	//	imshow("当前视频4", edges);

	//	if (waitKey(30) >= 0)
	//		stop = true;
	//}
	waitKey();
	return 0;
}

allRice getallRicedata(vector<Rice> rices){
	allRice allrice;
	allrice.areanum = rices.size();
	int l = rices.size();
	int activenum=0;
	for (int i = 0; i < l; i++){
		allrice.ricenum += rices.at(i).ricenum;
		if (rices.at(i).edge) allrice.edgenum += rices.at(i).ricenum;
		if (rices.at(i).link) allrice.linknum += rices.at(i).ricenum;
		if (rices.at(i).incomplete) allrice.incompletenum += rices.at(i).ricenum;
		if (rices.at(i).abandoned) continue;
		activenum += rices.at(i).ricenum;
		allrice.areasize += rices.at(i).areasize;
		allrice.roundness += rices.at(i).roundness;
		allrice.rectangularity += rices.at(i).rectangularity;
		allrice.lengthwidthratio += rices.at(i).minRectlengthwidthratio;
	}
	allrice.areasize /= activenum;
	allrice.roundness /= activenum;
	allrice.rectangularity /= activenum;
	allrice.lengthwidthratio /= activenum;
	return allrice;
}

Mat drawRect(Mat input, vector<Point> points)
{
	Mat out(input);
	for (int j = 0; j < 4; j++)
	{
		line(out, points.at(j), points.at((j+1)%4), Scalar(0,0,0), 1);
	}
	return out;
}