// xg-cmd.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{	
	CJudger j1;
	CWatcher w1("Watch",imread("src/test/0.jpg"));
	
	Mat bacground;
	cout << "=====¼ȡ����ͼ=====" << endl;
	imshow("����ͼ", Mat(100, 100, CV_8U));
	waitKey(0);
	w1.shotBackground();
	//system("cls");
	

	cout << "=====��ʼ����=====" << endl;
	bool stop = false;
	imshow("��ȡͼ",Mat(200,200,CV_8U));
	while (!stop)
	{
		switch (waitKey())
		{
			case 32:{//�ո�	�������
				vector<Mat> miuks = w1.shotItem();
				j1.judge(miuks);
				break;
			};
			case 98:{//b	���㱳��
				w1.shotBackground();
				break;
			}
			case 113:{//q	�˳�
				stop = true;
				break;
			};
			case 116:{//t	�����ȷ����
				vector<Mat> miuks = w1.shotItem();
				j1.trans_true(miuks);
				break;
			};
			case 102:{//f	��Ӵ��󼯺�
				vector<Mat> miuks = w1.shotItem();
				j1.trans_false(miuks);
				break;
			}
		}
	}
	
	return 0;
}