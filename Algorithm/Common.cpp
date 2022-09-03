
#define _USE_MATH_DEFINES
#include<algorithm>
#include"Common.h"
#include<string>
#include<sstream>
#include"Logger.h"

using namespace std;

std::vector<double> Common::R80List = { 1 ,1.03 ,1.06 ,1.09 ,1.12 ,1.15 ,1.18 ,1.22 ,1.25 ,1.28 ,1.32 ,1.36 ,1.4 ,1.45 ,1.5 ,1.55 ,1.6 ,1.65 ,1.7 ,1.75 ,1.8 ,1.85 ,1.9 ,1.95 ,
		2 ,2.06 ,2.12 ,2.18 ,2.24 ,2.3 ,2.36 ,2.43 ,2.5 ,2.58 ,2.65 ,2.72 ,2.8 ,2.9 ,3 ,3.07 ,3.15 ,3.25 ,3.35 ,3.45 ,3.55 ,3.65 ,3.75 ,3.87 ,
		4 ,4.12 ,4.25 ,4.37 ,4.5 ,4.62 ,4.75 ,4.87 ,5 ,5.15 ,5.3 ,5.45 ,5.6 ,5.8 ,6 ,6.15 ,6.3 ,6.5 ,6.7 ,6.9 ,7.1 ,7.3 ,7.5 ,7.75 ,
		8 ,8.25 ,8.5 ,8.75 ,9 ,9.25 ,9.5 ,9.75 };

Logger logger(R"(C:\111\logger.txt)", true);

vector<double> Common::GenerateSweepFreq(double startHz, double stopHz, Octave oct, double** output)
{
	if (startHz <= 0 || stopHz <= 0)
	{
		throw std::invalid_argument("startHz or stopHz must bigger than zero");

	}

	int Interval = static_cast<int>(log2(int(oct)))+1;

	double start = startHz,stop = stopHz;
	if (startHz > stopHz)
	{
		start = stopHz;
		stop = startHz;
	}
	int exp = int(log10(start));
	int coefficient;
	std::vector<double> res;

	double freq = start;
	int index = 0;
	int Len = R80List.size();
	while (freq < stop)
	{
		coefficient = static_cast<int>(pow(10, exp++));

		for (index =0 ; index < Len; index += Interval) 
		{
			freq = R80List[index] * coefficient;

			if (freq > stop)
			{
				break;
			}

			if (freq < start)
			{
				continue;
			}
			res.push_back(round(freq*100)/100);   //浮点形导致的精度细微差异，为了对标暂时这么写。 实际使用中应该没有差异。具体看应用
		}
	}

	if (startHz > stopHz)
	{
		vector<double> vec(res.crbegin(), res.crend());
		res = vec;
	}


	return res;

}

/// <summary>
///  需要的参数，根据最小持续时间计算出最大持续周期数，和最小周期相比，取大的
///  计算出当前频率需要的点数
/// 计算出当前频率持续的时间
/// </summary>
/// <param name="startHz"></param>
/// <param name="stopHz"></param>
/// <param name="minCycle"></param>
/// <param name="minDuration"></param>
/// <param name="oct"></param>
/// 
void Common::GenerateSweepWave(double startHz, double stopHz, int minCycle, int minDuration, Octave oct)
{
	//最小持续时间单位是毫秒
	int SampleRate = 48000;
	vector<double> freqlst = GenerateSweepFreq(startHz, stopHz, oct, nullptr);
	vector<SweepInfo> infos;
	
	int SampleStart = 0;
	double DurationStart = 0;
	for (double freq : freqlst)
	{
		double Cycle = minDuration*freq / 1000;  //计算最小持续时间下的周期数, 时间/周期= 周期数

		int nCycle = 0;
		if (minCycle < Cycle)
		{
			nCycle = static_cast<int>(Cycle);   //每个频点循环的周期数,四舍五入
			if (Cycle - nCycle > 0.5)
			{
				nCycle++;
			}
			if (nCycle > minCycle * 400) { nCycle = minCycle * 400; };
		}
		
		nCycle = max<int>(nCycle, minCycle);   //取较大的
		
		SweepInfo info;
		info.Freq = freq;
		info.Cycle = nCycle;
		info.Sample = static_cast<int>(info.Cycle*1.0 * SampleRate/freq + 0.5);  //采样点数
		info.Duration = info.Sample*1.0 / SampleRate;   //每个频点持续的时间
		
		string Msg = "Freq: " + to_string(info.Freq) + " Cycle: " + to_string(info.Cycle) + " Sample: " + to_string(info.Sample)
			+ " Duration: " + to_string(info.Duration) + " DurationStart: " + to_string(DurationStart) + " SampleStart: "+to_string(SampleStart);
		//cout << Msg << endl;
		logger.Debug(Msg);
		SampleStart += info.Sample;
		DurationStart += info.Duration;
		infos.push_back(info);
	}
	string Msg = " ************* DurationStart: " + to_string(DurationStart) + " SampleStart: " + to_string(SampleStart);
	logger.Debug(Msg);
	int ExtenLen = 2048;
	 Msg = " ************* TotalDuration: " + to_string(DurationStart) + " TotalSample: " + to_string(SampleStart+ExtenLen+1);
	logger.Debug(Msg);

	vector<double> wavedata;
	double Q = 0;

	std::stringstream fmt;
	for (auto const& info : infos)
	{
		for (int j = 0; j < info.Sample; j++)
		{
			fmt.clear();
			double t = 1.0 * j * info.Freq / SampleRate + Q;
			double val = sin(2 * M_PI * t);
			wavedata.push_back(val);
			//Msg =" Freq: "+to_string(info.Freq)+" index: "+ to_string(j)+ "Time: " + to_string(t) + " val: " + to_string(val) + " Q: " + to_string(Q);
			fmt << "Freq: " << info.Freq << " index: " << j << " Time: " << t << " val: " << val << " Q: " << Q;
			logger.Debug(fmt.str());
			fmt.str("");
		}

		double realCycle = 1.0* info.Sample * info.Freq / SampleRate + Q;

		Q = realCycle - info.Cycle;	
	}



}


