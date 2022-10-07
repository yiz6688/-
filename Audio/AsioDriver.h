#pragma once

#include<iostream>
#include"Asio/iasiodrv.h"
#include"Asio/asio.h"
#include<vector>

using namespace std;
/// <summary>
/// 由于底层sdk使用了c接口的回调函数
/// 目前实现方案为根据输入输出接口数量
/// 动态生成相应的数组来实现调用
/// 多数内容使用静态方法
/// </summary>

#define MAXPATHLEN			512
#define MAXDRVNAMELEN		128

struct DriverInfo
{
	CLSID clsid;
	char drvname[MAXDRVNAMELEN];
	IASIO*	asiodrv;
};



struct InputBufferInfo
{

};

struct OutputBufferInfo
{


};


/// <summary>
/// 受限于底层框架，当前类只能操作一个asio设备
/// </summary>

class Asio
{

public:
	typedef void(*AudioAvailable)();

public:
	static void Init();

	static void OpenDriver(int drvID);

	static void CloseDriver();


	static void GetInputChannels();

	static int GetOutputChannels();

	static int GetInputLatency();

	static int GetOutputLatency();

	static ASIOError GetSampleRate();

	static ASIOError SetSampleRate();

	static string GetErrorMessage();

	static ASIOError StartPlayBack(int nChannel, AudioAvailable callback);

	static ASIOError StopPlayBack(int nChannel);

	static ASIOError StartCapture(int nChannel, AudioAvailable callback);

	static ASIOError StopCapture(int nChannel);

private:



	static int getChannels();

	static int getLatencies();

	static void getDriverList();

private:
	static void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess);

	static void sampleRateDidChange(ASIOSampleRate sRate);

	static long asioMessage(long selector, long value, void* message, double* opt);

	static ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);



private:
	static IASIO *iasio;

	static long inputChannels;

	static long outputChannels;

	static long minSize;

	static long maxSize;

	static long preferredSize;

	static long granularity;

	static ASIOSampleRate sampleRate;

	static long inputLatency;

	static long outputLatency;

	static long inputBuffers;

	static long outputBuffers;

	static vector<ASIOBufferInfo> bufferInfos;

	static vector<ASIOChannelInfo> channelInfos;

	static vector<DriverInfo> driverInfos;
};

IASIO* Asio::iasio;

long Asio::inputChannels;

long Asio::outputChannels;

long Asio::minSize;

long Asio::maxSize;

long Asio::preferredSize;

long Asio::granularity;

ASIOSampleRate Asio::sampleRate;

long Asio::inputLatency;

long Asio::outputLatency;

long Asio::inputBuffers;

long Asio::outputBuffers;

vector<ASIOBufferInfo> Asio::bufferInfos;

vector<ASIOChannelInfo> Asio::channelInfos;

vector<DriverInfo> Asio::driverInfos;