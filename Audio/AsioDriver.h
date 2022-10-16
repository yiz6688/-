#pragma once

#include<iostream>
#include"Asio/iasiodrv.h"
#include"Asio/asio.h"
#include<vector>
#include "AsioInput.h"
#include "AsioOutput.h"
#include <mutex>
#include "AsioAudioOutputAvaliableArgs.h"

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
public:
	DriverInfo()
	{
		asiodrv = nullptr;
		memset(drvname, 0, MAXDRVNAMELEN);
		memset(&clsid, 0, sizeof(clsid));
	}

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



typedef void(*AudioOutputAvailable)(AsioAudioOutputAvaliableArgs args);
/// <summary>
/// 受限于底层框架，当前类只能操作一个asio设备
/// 先实现单设备的操作
/// </summary>

class AsioDriver
{

public:


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

	static ASIOError StartPlayBack(int channelOffset, int channenNum, AudioInputAvailable callback);

	static ASIOError StopPlayBack(int channelOffset);

	static ASIOError StartCapture(int nChannel, AudioOutputAvailable callback);

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


	static bool hasRuning();


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

	static vector<AsioInput> AsioInputs;

	static vector<AsioOutput> AsioOutputs;

	static mutex m;

};

