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
	//Ƶ��
	double Freq;
	//������
	int Cycle;
	//����ʱ��
	double Duration;
	//��������
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








