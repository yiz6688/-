#pragma once
#include<iostream>
#include"curveh.h"
using namespace std;
class doubleCurve : public Curve<double>
{



private:
	vector<point<double,double>>  arr;
	int nt;


	/// <summary>
	/// 计算向量ab 和 cd 的叉积
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <param name="d"></param>
	/// <returns></returns>
	double line_cross_product(point<double, double> a, point<double, double>b,
		point<double, double> c, point<double, double> d);

public:
	doubleCurve(vector<double> x_lst, vector<double> y_lst);

	doubleCurve(double* arr_x, int xlen, double* arr_y, int ylen);

	doubleCurve(const doubleCurve& dc);

	doubleCurve(int a) { nt = a; };
	void cmp(Curve<double>& c);

	void opera(Curve<double>& c, func f) {};
};