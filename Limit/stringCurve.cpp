#include"stringCurve.h"
#include <iostream>

using namespace std;

stringCurve::stringCurve(vector<std::string> x_lst, vector<double> y_lst)
{
	int xlen = x_lst.size();
	int ylen = y_lst.size();
	if ( xlen != ylen)
	{
		throw std::invalid_argument::invalid_argument("无效的入参");
	}

	for (int i = 0; i < xlen; i++)
	{
		_Xlst.push_back(x_lst[i]);
		mp[x_lst[i]] = y_lst[i];
	}

}

stringCurve::stringCurve(std::string* arr_x, double* arr_y, int arrlen)
{
	for (int i = 0; i < arrlen; i++)
	{
		_Xlst.push_back(arr_x[i]);
		mp[arr_x[i]] = arr_y[i];
	}
}

stringCurve::stringCurve(const stringCurve& dc)
{
}

void stringCurve::CurveCmp(Curve<std::string>& c)
{
	stringCurve &sc = static_cast<stringCurve&>(c);

	std::map<std::string, double>::iterator sciter;
	for (auto iter = mp.begin(); iter != mp.end(); iter++)
	{
		sciter = sc.mp.find(iter->first);

		if (sciter != sc.mp.end())
		{
			if (sciter->second > iter->second)
			{
				cout << 0 << endl;
			}
			else
			{
				cout << 1 << endl;
			}
		}

	}





}

void stringCurve::opera(Curve<std::string>& c, func f)
{
	stringCurve& sc = static_cast<stringCurve&>(c);

	std::map<std::string, double>::iterator sciter;

	for (auto iter = mp.begin(); iter != mp.end(); iter++)
	{
		sciter = sc.mp.find(iter->first);

		if (sciter != sc.mp.end())
		{
			iter->second = f(&iter->second, &sciter->second);
		}

	}
}

std::string stringCurve::Conver(std::string x)
{
	return "";
}
