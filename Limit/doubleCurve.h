#pragma once
#include<iostream>
#include"curveh.h"
using namespace std;
class doubleCurve : public Curve<double>
{



private:
	vector<point<double,double>>  arr;

	string _type;

	/// <summary>
	/// 两条线段进行叉积运算，用来判断线段间的相对位置关系以及是否相交
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <param name="d"></param>
	/// <returns>
	/// 0  相交
	/// 1  ab 在 cd下方
	/// -1 ab在 cd上方
	/// </returns>
	double line_cross_product(point<double, double> a, point<double, double>b,
		point<double, double> c, point<double, double> d);

	double GetPointFromLine(point<double, double>& a, point<double, double>& b, point<double, double>& c);
	

public:
	static void Test();

public:
	doubleCurve(vector<double> x_lst, vector<double> y_lst);

	doubleCurve(double* arr_x, double* arr_y, int arrlen);

	doubleCurve(const doubleCurve& dc);

	void CurveCmp(Curve<double>& c);

	void ScatterCmp(Curve<double>& c);

	double Conver(double x);

	point<double, double> Conver(point<double, double> p);

	void opera(Curve<double>& c, func f) ;
};