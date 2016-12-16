#include "stdafx.h"
#include "Judger.h"

CJudger::CJudger(){
	true_set = new CImgSetData();
	false_set = new CImgSetData();
}

CJudger::CJudger(string true_set_path,string false_set_path)
{
	cout << "���ؼ��Ա��" << endl;

	cout << "=========�����ܼ���=========" << endl;
	true_set = new CImgSetData(true_set_path);
	cout << "����ƽ�� : " << true_set->getLength("avg") << endl;
	cout << "���ֵ : " << true_set->getLength("max") << endl;
	cout << "��Сֵ : " << true_set->getLength("min") << endl<<endl;

	cout << "=========�����ܼ���=========" << endl;
	false_set = new CImgSetData(false_set_path);
	cout << "����ƽ�� : " << false_set->getLength("avg") << endl;
	cout << "���ֵ : " << false_set->getLength("max") << endl;
	cout << "��Сֵ : " << false_set->getLength("min") << endl;

	cout << "���ؼ��Ա�������" << endl;
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
			cout << "������";
		}
		else if (true_s == 0 && false_s == 1){
			cout << "������";
		}
		else{
			cout << "�޷��ж�";
		}
		cout << "    ��ȷ��ʶ�ȣ�" << true_s << "������ʶ�ȣ�" << false_s << "---" << i << endl;
		imshow("���ͼ", x);
		waitKey(0);
	}
	cv::destroyWindow("���ͼ");
}

void CJudger::trans_true(vector<Mat> imgs){
	true_set->addTrans(imgs);
}
void CJudger::trans_false(vector<Mat> imgs){
	false_set->addTrans(imgs);
}