#pragma once
#include<map>
#include<string>
#include"Curveh.h"

class stringCurve :public Curve<std::string>
{
public:
	stringCurve(vector<std::string> x_lst, vector<double> y_lst);

	stringCurve(std::string* arr_x, double* arr_y, int arrlen);

	stringCurve(const stringCurve& dc);

	void CurveCmp(Curve<std::string>& c);

	void opera(Curve<std::string>& c, func f);

	std::string Conver(std::string x);

private:
	//ʹ��vector��֤����ʹ��map���ٵ���
	std::vector<std::string> _Xlst;
	std::map<std::string, double> mp;

};
