#include "doubleCurve.h"
#include<algorithm>




double doubleCurve::line_cross_product(point<double, double> a, point<double, double> b, point<double, double> c, point<double, double> d)
{
	//叉积负数表示在逆时针方向，正数表示在顺时针方向， 0表示在同一条线上
	//负数表示b在oa逆时针方向 正数表示b在 oa顺时针方向 0表示在同一直线
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
	
	if ((ret1 > 0 && ret2 > 0) || (ret3 < 0 && ret4 < 0))  //cd在 ab逆时针方向 或者 ab在cd顺时针方向
	{
		return -1;
	}
	else if ((ret1 < 0 && ret2 < 0) || (ret3 > 0 && ret4 > 0)) //cd在ab顺时针方向 或者 ab在cd逆时针方向
	{
		return 1;
	}
	else   //相交
	{
		return 0;
	}
	return 0.0;
}

/// <summary>
/// 从ab线段内取 c横坐标映射的值
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
double doubleCurve::GetPointFromLine(point<double, double>& a, point<double, double>& b, point<double, double>& c)
{
	if (a.X > c.X || b.X < c.X)
	{
		throw std::invalid_argument::invalid_argument("不在范围内");
	}

	double x1 = Conver(a.X);
	double y1 = a.Y;
	
	double x2 = Conver(b.X);
	double y2 = b.Y;

	double x = Conver(c.X);

	double coeff = (y2 - y1) / (x2 - x1);

	return (x - x2) * coeff + y2;
}



void doubleCurve::Test()
{

	double freq[] = { 20,21.2,22.4,23.6,25,26.5,28,30,33.5,35.5,37.5,40,42.5,45,47.5,50,53,56,60,63,67,
	71,75,80,85,90,95,100,106,112,118,125,132,140,150,160,170,190,200 };

	double up[] = { 120,121,123,123,124,124,125,125,126,126,127,127,127,128,128,128,129,129,130,130,129,
		129,128,128.00 ,127,127,126,126,125,125,124,124,123,123,122,122,120,120,120 };

	double spl1[] = { 118,119,121,121,122,122,123,123,124,124,125,125,125,126,126,126,127,127,128,128,127,127,
		126,126,125,125,124,124,123,123,122,122,121,121,120,120,118,118,118
	};

	double low[] = { 116,117,119,119,120,120,121,121,122,122,123,123,123,124,124,124,125,125,126,126,125,125,
		124,124,123,123,122,122,121,121,120,120,119,119,118,118,116,116,116
	};

	double freq2[] = { 10,30,31,32,59,72,189 };

	double lmt2[] = { 121,115,131,114,135,124,130 };


	doubleCurve dc(freq, spl1, 39);
	doubleCurve dcup(freq, up, 39);
	doubleCurve dclow(freq, low, 39);

	doubleCurve lmt(freq2, lmt2, 7);

	//cout << "judge Up" << endl;

	//dc.CurveCmp(dcup);

	//cout << "judge Low" << endl;

	//dc.CurveCmp(dclow);

	//cout << "judge Lmt2" << endl;

	//dc.CurveCmp(lmt);

	double freq3[] = { 3200,4000,4800,5600,6400 };
	double oct[] = { -118,-119,-112,-118,-117 };

	double octup[] = { -115,-115,-115,-115,-115 };

	doubleCurve octc(freq3, oct,5);
	doubleCurve octup1(freq3, octup, 5);
	octc.ScatterCmp(octup1);


}

doubleCurve::doubleCurve(vector<double> x_lst, vector<double> y_lst)
{
	_type = "log";
	int xlen = x_lst.size();
	int ylen = y_lst.size();
	if (xlen < 2 || ylen < 2 || xlen != ylen)
	{
		throw std::invalid_argument::invalid_argument("无效的入参");
	}

	
	for (int i = 0; i < xlen; i++)
	{
		point<double, double> p(x_lst[i], y_lst[i]);
		arr.push_back(p);
	}


}

doubleCurve::doubleCurve(double* arr_x, double* arr_y, int arrlen)
{
	if (arrlen < 2)
	{
		throw std::invalid_argument::invalid_argument("无效的入参");
	}
	for (int i = 0; i < arrlen; i++)
	{
		point<double, double> p(arr_x[i], arr_y[i]);
		arr.push_back(p);
	}
}

doubleCurve::doubleCurve(const doubleCurve& dc)
{
}




void doubleCurve::CurveCmp(Curve<double>& c)
{
	doubleCurve &dc = static_cast<doubleCurve&>(c);

	cout << typeid(c).name() << endl;

	point<double, double> *pa1, *pa2, *pb1, *pb2;
	int index1 = 0, index2 = 0;
	int len1 = this->arr.size();
	int len2 = dc.arr.size();

	//点坐标判断，三种返回值 -1 表示小于  1表示大于  0表示相等
	auto pointCmp = [](point<double, double> p1, point<double, double> p2)
	{
		if (p1.X - p2.X < 1e-8 || p2.X - p1.X < 1e-8)
		{
			return 0;
		}
		else if (p1.X > p2.X)
		{
			return 1;
		}
		else
		{
			return -1;
		}

	};


	pa1 = &this->arr[index1++];
	pa2 = &this->arr[index1++];

	pb1 = &dc.arr[index2++];
	pb2 = &dc.arr[index2++];


	while (true)
	{
		//判断线段在X轴上的映射相交，如果相交的话一定有公共部分，必然满足一下条件。
		//没有计入等于，是因为如果有点重叠的时候会重复计算。
		if (pa1->X < pb2->X && pa2->X > pb1->X)
		{
			double val = line_cross_product(*pa1, *pa2, *pb1, *pb2);   //计算两个线段的位置关系
			cout << pa1->X << " ret:" << val << endl;
		}

		//判断完毕以后需要向右边移动线段，
		//移动的标准，线段右边界点小的移动，如果相同，

		if (pa2->X < pb2->X)
		{
			if (index1 == len1)
			{
				break;
			}
			pa1 = pa2;
			pa2 = &(this->arr[index1++]);
		}
		else
		{
			if (index2 == len2)
			{
				break;
			}
			pb1 = pb2;
			pb2 = &(dc.arr[index2++]);
		}


	}


}

void doubleCurve::ScatterCmp(Curve<double>& c)
{
	doubleCurve& dc = static_cast<doubleCurve&>(c);

	cout << typeid(c).name() << endl;

	point<double, double>* pa1, * pa2, * pb1, * pb2;
	int index1 = 0, index2 = 0;
	int len1 = this->arr.size();
	int len2 = dc.arr.size();

	//点坐标判断，三种返回值 -1 表示小于  1表示大于  0表示相等
	auto pointCmp = [](point<double, double> p1, point<double, double> p2)
	{
		if (p1.X - p2.X < 1e-8 || p2.X - p1.X < 1e-8)
		{
			return 0;
		}
		else if (p1.X > p2.X)
		{
			return 1;
		}
		else
		{
			return -1;
		}

	};


	pa1 = &this->arr[index1++];
	//pa2 = &this->arr[index1++];

	pb1 = &dc.arr[index2++];
	pb2 = &dc.arr[index2++];


	while (true)
	{
		//点在线段范围上
		if (pa1->X <= pb2->X && pa1->X >= pb1->X)
		{
			double val = GetPointFromLine(*pb1, *pb2, *pa1);

			cout << pa1->X << " ret:" << (pa1->Y> val) << endl;

		}

		//判断完毕以后需要向右边移动线段，
		//移动的标准，线段右边界点小的移动，如果相同，

		if (pa1->X <= pb2->X)
		{
			if (index1 == len1)
			{
				break;
			}
			//pa1 = pa2;
			pa1 = &(this->arr[index1++]);
		}
		else
		{
			if (index2 == len2)
			{
				break;
			}
			pb1 = pb2;
			pb2 = &(dc.arr[index2++]);
		}





	}



}

point<double, double> doubleCurve::Conver(point<double, double> p)
{
	  point<double, double> p1(log10(p.X), p.Y);
	  return p1;
}

void doubleCurve::opera(Curve<double>& c, func f)
{

	doubleCurve& dc = static_cast<doubleCurve&>(c);

	cout << typeid(c).name() << endl;

	point<double, double>* pa1, * pa2, * pb1, * pb2;
	int index1 = 0, index2 = 0;
	int len1 = this->arr.size();
	int len2 = dc.arr.size();

	//点坐标判断，三种返回值 -1 表示小于  1表示大于  0表示相等
	auto pointCmp = [](point<double, double> p1, point<double, double> p2)
	{
		if (p1.X - p2.X < 1e-8 || p2.X - p1.X < 1e-8)
		{
			return 0;
		}
		else if (p1.X > p2.X)
		{
			return 1;
		}
		else
		{
			return -1;
		}

	};


	pa1 = &this->arr[index1++];
	//pa2 = &this->arr[index1++];

	pb1 = &dc.arr[index2++];
	pb2 = &dc.arr[index2++];


	while (true)
	{
		//点在线段范围上
		if (pa1->X <= pb2->X && pa1->X >= pb1->X)
		{
			double val = GetPointFromLine(*pb1, *pb2, *pa1);

			pa1->Y = f(&pa1->Y, &val);

		}

		//判断完毕以后需要向右边移动线段，
		//移动的标准，线段右边界点小的移动，如果相同，

		if (pa1->X <= pb2->X)
		{
			if (index1 == len1)
			{
				break;
			}
			//pa1 = pa2;
			pa1 = &(this->arr[index1++]);
		}
		else
		{
			if (index2 == len2)
			{
				break;
			}
			pb1 = pb2;
			pb2 = &(dc.arr[index2++]);
		}





	}



}

double doubleCurve::Conver(double x)
{
	if (_type == "log10")
	{
		return log10(x);
	}
	return x;
}


