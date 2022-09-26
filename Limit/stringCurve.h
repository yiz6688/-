#pragma once
#include<map>
#include<string>
#include"Curveh.h"

class stringCurve :public Curve<std::string>
{
public:
	stringCurve();

	void CurveCmp(Curve<std::string>& c) {};

	void opera(Curve<std::string>& c, func f) {};

	std::string Conver(std::string x);

private:
	//使用vector保证有序，使用map快速迭代
	std::vector<std::string> _Xlst;
	std::map<std::string, double> mp;

};
