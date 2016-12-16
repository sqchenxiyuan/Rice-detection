#include "stdafx.h"
#include "Judger.h"

CJudger::CJudger(){
	true_set = new CImgSetData();
	false_set = new CImgSetData();
}

CJudger::CJudger(string true_set_path,string false_set_path)
{
	cout << "加载检测员中" << endl;

	cout << "=========有吸管集合=========" << endl;
	true_set = new CImgSetData(true_set_path);
	cout << "错误平均 : " << true_set->getLength("avg") << endl;
	cout << "最大值 : " << true_set->getLength("max") << endl;
	cout << "最小值 : " << true_set->getLength("min") << endl<<endl;

	cout << "=========无吸管集合=========" << endl;
	false_set = new CImgSetData(false_set_path);
	cout << "错误平均 : " << false_set->getLength("avg") << endl;
	cout << "最大值 : " << false_set->getLength("max") << endl;
	cout << "最小值 : " << false_set->getLength("min") << endl;

	cout << "加载检测员加载完成" << endl;
	//Sleep(1000);
	//system("Cls");
}


CJudger::~CJudger()
{
}

void CJudger::judge(vector<Mat> block){
	for (int i = 0; i < block.size(); i++)
	{
		Mat x = block.at(i);
		float true_s = true_set->getAcquaintance(x);
		float false_s = false_set->getAcquaintance(x);

		if (true_s == 1 && false_s == 0){
			cout << "有吸管";
		}
		else if (true_s == 0 && false_s == 1){
			cout << "无吸管";
		}
		else{
			cout << "无法判断";
		}
		cout << "    正确相识度：" << true_s << "错误相识度：" << false_s << "---" << i << endl;
		imshow("监测图", x);
		waitKey(0);
	}
	cv::destroyWindow("监测图");
}

void CJudger::trans_true(vector<Mat> imgs){
	true_set->addTrans(imgs);
}
void CJudger::trans_false(vector<Mat> imgs){
	false_set->addTrans(imgs);
}