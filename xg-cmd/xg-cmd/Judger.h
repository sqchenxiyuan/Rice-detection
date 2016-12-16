#pragma once
class CJudger
{
public:
	CJudger();
	CJudger(string true_set_path, string false_set_path);
	~CJudger();
	void trans_true(vector<Mat> imgs);
	void trans_false(vector<Mat> imgs);
	void judge(vector<Mat> block);
private:
	CImgSetData* true_set;
	CImgSetData* false_set;
};

