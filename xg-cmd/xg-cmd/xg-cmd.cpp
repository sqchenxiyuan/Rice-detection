// xg-cmd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{	
	CJudger j1;
	CWatcher w1("Watch",imread("src/test/0.jpg"));
	
	Mat bacground;
	cout << "=====录取背景图=====" << endl;
	imshow("背景图", Mat(100, 100, CV_8U));
	waitKey(0);
	w1.shotBackground();
	//system("cls");
	

	cout << "=====开始运行=====" << endl;
	bool stop = false;
	imshow("提取图",Mat(200,200,CV_8U));
	while (!stop)
	{
		switch (waitKey())
		{
			case 32:{//空格	拍摄分析
				vector<Mat> miuks = w1.shotItem();
				j1.judge(miuks);
				break;
			};
			case 98:{//b	拍摄背景
				w1.shotBackground();
				break;
			}
			case 113:{//q	退出
				stop = true;
				break;
			};
			case 116:{//t	添加正确集合
				vector<Mat> miuks = w1.shotItem();
				j1.trans_true(miuks);
				break;
			};
			case 102:{//f	添加错误集合
				vector<Mat> miuks = w1.shotItem();
				j1.trans_false(miuks);
				break;
			}
		}
	}
	
	return 0;
}