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
}


CJudger::~CJudger()
{
}

void CJudger::judge(vector<Mat> block){
	clock_t startTime, endTime;
	startTime = clock();
	for (int i = 0; i < block.size(); i++)
	{
		Mat x = block.at(i);
		float true_s = true_set->getAcquaintance(x);
		float false_s = false_set->getAcquaintance(x);

		if (true_s <= 0 && false_s <= 0){
			cout << "�޷��ж�";
		}
		else if (true_s >= false_s){
			cout << "������";
		}
		else if (true_s < false_s){
			cout << "������";
		}
		else{
			cout << "�޷��ж�";
		}
		cout << "    ��ȷ��ʶ�ȣ�" << true_s << "������ʶ�ȣ�" << false_s << "---" << i << endl;
	}
	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

void CJudger::trans_true(vector<Mat> imgs){
	clock_t startTime, endTime;
	startTime = clock();
	true_set->addTrans(imgs);
	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}
void CJudger::trans_false(vector<Mat> imgs){
	clock_t startTime, endTime;
	startTime = clock();
	false_set->addTrans(imgs);
	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}