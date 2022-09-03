#pragma once
#include<vector>
#include<stdexcept>
#include<math.h>
#include<iostream>

using namespace std;
enum class Octave
{
	R10 = 8,
	R20 = 4,
	R40 = 2,
	R80 = 1
};

struct SweepInfo
{
	//频率
	double Freq;
	//周期数
	int Cycle;
	//持续时间
	double Duration;
	//采样点数
	int Sample;
};



class Common
{

private:
	static std::vector<double> R80List;

public:
	static vector<double> GenerateSweepFreq(double startHz, double stopHz, Octave oct, double **output);

	static void GenerateSweepWave(double startHz, double stopHz, int minCycle, int minDuration, Octave oct);



};








