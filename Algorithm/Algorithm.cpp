// Algorithm.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"Common.h"
#include <iostream>


#include<iostream>
using namespace std;




int main()
{
	double startHz = 20, stopHz = 20000;
	//vector<double> res = Common::GenerateSweepFreq(startHz, stopHz, Octave::R40, nullptr);
	Common::GenerateSweepWave(stopHz, startHz,10,10, Octave::R40);

	return 0;
}
