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
	/// �����߶ν��в�����㣬�����ж��߶μ�����λ�ù�ϵ�Լ��Ƿ��ཻ
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <param name="d"></param>
	/// <returns>
	/// 0  �ཻ
	/// 1  ab �� cd�·�
	/// -1 ab�� cd�Ϸ�
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