#include "doubleCurve.h"
#include<algorithm>


double doubleCurve::line_cross_product(point<double, double> a, point<double, double> b, point<double, double> c, point<double, double> d)
{
	//叉积负数表示在逆时针方向，正数表示在顺时针方向， 0表示在同一条线上
	auto cross_product = [](point<double, double> o, point<double, double> a, point<double, double> b)
	{
		//对数坐标轴下，要对x取对数
	/*double x1 = a.X - o.X;
	double y1 = a.Y - o.Y;
	double x2 = b.X - o.X;
	double y2 = b.Y - o.Y;*/

		double x1 = log10(a.X / o.X);
		double y1 = a.Y - o.Y;
		double x2 = log10(b.X / o.X);
		double y2 = b.Y - o.Y;

		return x1 * y2 - y1 * x2;
	};

	double ret1 = cross_product(a, b, c);
	double ret2 = cross_product(a, b, d);
	double ret3 = cross_product(c, d, a);
	double ret4 = cross_product(c, d, b);

	if ((ret1 > 0 && ret2 > 0) || (ret3 < 0 && ret4 < 0))
	{
		return 1;
	}
	else if ((ret1 < 0 && ret2 < 0) || (ret3 > 0 && ret4 > 0))
	{
		return -1;
	}
	else
	{
		return 0;
	}
	return 0.0;
}

doubleCurve::doubleCurve(vector<double> x_lst, vector<double> y_lst)
{
}

doubleCurve::doubleCurve(double* arr_x, int xlen, double* arr_y, int ylen)
{
}

doubleCurve::doubleCurve(const doubleCurve& dc)
{
}

void doubleCurve::cmp(Curve<double>& c)
{
	doubleCurve &dc = dynamic_cast<doubleCurve&>(c);

	cout << typeid(c).name() << endl;
}


