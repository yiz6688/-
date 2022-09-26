#include "doubleCurve.h"
#include<algorithm>



double doubleCurve::line_cross_product(point<double, double> a, point<double, double> b, point<double, double> c, point<double, double> d)
{
	//���������ʾ����ʱ�뷽��������ʾ��˳ʱ�뷽�� 0��ʾ��ͬһ������
	auto cross_product = [](point<double, double> o, point<double, double> a, point<double, double> b)
	{
		//�����������£�Ҫ��xȡ����
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

double doubleCurve::GetPointFromLine(point<double, double>& a, point<double, double>& b, point<double, double>& c)
{
	if (a.X > c.X || b.X < c.X)
	{
		throw std::invalid_argument::invalid_argument("���ڷ�Χ��");
	}

	double x1 = Conver(a.X);
	double y1 = a.Y;
	
	double x2 = Conver(b.X);
	double y2 = b.Y;

	double x = Conver(c.X);

	double coeff = (y2 - y1) / (x2 - x1);

	return (x - x2) * coeff + y2;
}



doubleCurve::doubleCurve(vector<double> x_lst, vector<double> y_lst)
{
	_type = "log";
	int xlen = x_lst.size();
	int ylen = y_lst.size();
	if (xlen < 2 || ylen < 2 || xlen != ylen)
	{
		throw std::invalid_argument::invalid_argument("��Ч�����");
	}

	
	for (int i = 0; i < xlen; i++)
	{
		point<double, double> p(x_lst[i], y_lst[i]);
		arr.push_back(p);
	}


}

doubleCurve::doubleCurve(double* arr_x, int xlen, double* arr_y, int ylen)
{
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

	//�������жϣ����ַ���ֵ -1 ��ʾС��  1��ʾ����  0��ʾ���
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
		//�ж��߶���X���ϵ�ӳ���ཻ������ཻ�Ļ�һ���й������֣���Ȼ����һ��������
		//û�м�����ڣ�����Ϊ����е��ص���ʱ����ظ����㡣
		if (pa1->X < pb2->X && pa2->X > pb1->X)
		{
			double val = line_cross_product(*pa1, *pa2, *pb1, *pb2);   //���������߶ε�λ�ù�ϵ
		}

		//�ж�����Ժ���Ҫ���ұ��ƶ��߶Σ�
		//�ƶ��ı�׼���߶��ұ߽��С���ƶ��������ͬ��

		if (pa2->X > pb2->X)
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
			pb2 = &(this->arr[index2++]);
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

	//�������жϣ����ַ���ֵ -1 ��ʾС��  1��ʾ����  0��ʾ���
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
		//�����߶η�Χ��
		if (pa1->X <= pb2->X && pa1->X >= pb1->X)
		{
			
		}

		//�ж�����Ժ���Ҫ���ұ��ƶ��߶Σ�
		//�ƶ��ı�׼���߶��ұ߽��С���ƶ��������ͬ��



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
			pb2 = &(this->arr[index2++]);
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


