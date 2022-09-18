#pragma once
#include<string>
#include"Curveh.h"

class stringCurve :public Curve<std::string>
{
public:
	stringCurve();

	void cmp(Curve<std::string>& c) {};

	void opera(Curve<std::string>& c, func f) {};
};
