#pragma once
#include<vector>
using std::vector;







template<typename T1, typename T2>
struct point
{
	point(T1 _X, T2  _Y)
	{
		this->X = _X;
		this->Y = _Y;
	}

	T1 X;
	T2 Y;
};




template<typename T>
class Curve
{
public:

	//std::numeric_limits<double>::epsilon(); // 这个就是

public:
	typedef double (*func)(double*, double*);
	
	//坐标轴底数
	typedef double (*root)(double);


public:
	virtual void CurveCmp(Curve<T>& c) = 0;

	virtual void opera(Curve<T>& c, func f) = 0;

	virtual void addval(double val) {};

	//virtual void vector<T> getXlst() {};

	//virtual void vector<double> getYlst() {};

	virtual T Conver(T x) = 0;

};