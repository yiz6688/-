#include "AsioDriver.h"
#include<Windows.h>

#define ASIODRV_DESC		"description"
#define INPROC_SERVER		"InprocServer32"
#define ASIO_PATH			"software\\asio"
#define COM_CLSID			"clsid"
#define MAXPATHLEN			512
#define MAXDRVNAMELEN		128

#define DRVERR			-5000
#define DRVERR_INVALID_PARAM		DRVERR-1
#define DRVERR_DEVICE_ALREADY_OPEN	DRVERR-2
#define DRVERR_DEVICE_NOT_FOUND		DRVERR-3


void Asio::Init()
{
	getDriverList();

	if (driverInfos.empty() == false) CoInitialize(0);	// 初始化COM
}

void Asio::OpenDriver(int drvID)
{
	if (drvID > driverInfos.size())
	{
		//return DRVERR_DEVICE_NOT_FOUND;
	}

	DriverInfo info = driverInfos.at(drvID);   //获取对应的驱动信息，这里没有进行判断处理

	LPVOID* asiodrv;

	HRESULT res;

	if (info.asiodrv == nullptr)
	{
		res = CoCreateInstance(info.clsid, 0, CLSCTX_INPROC_SERVER, info.clsid, (LPVOID*)(&info.asiodrv));

		if (res == S_OK)
		{
			//info.asiodrv = (IASIO*)(*asiodrv);
		}
	}
	else
	{
		//return DRVERR_DEVICE_ALREADY_OPEN;
	}
	
	void* handle = nullptr;
	iasio->init(handle);   //初始化驱动

	char name[32];
	iasio->getDriverName(name);

	iasio->getBufferSize(&minSize , &maxSize, &preferredSize , &granularity);   //获取缓冲区

	iasio->getLatencies(&inputLatency, &outputLatency);   //获取延时

	iasio->getChannels(&inputChannels, &outputChannels);   //获取通道数

	

	for (int i = 0; i < inputChannels; i++)
	{
		ASIOChannelInfo  channelinfo;
		ASIOBufferInfo bufferinfo;

		channelinfo.isInput = ASIOTrue;
		channelinfo.channel = i;

		bufferinfo.isInput = ASIOTrue;
		bufferinfo.channelNum = i;

		iasio->getChannelInfo(&channelinfo);

		channelInfos.push_back(channelinfo);
		bufferInfos.push_back(bufferinfo);
	}

	for (int i = 0; i < outputChannels; i++)
	{
		ASIOChannelInfo  channelinfo;
		ASIOBufferInfo bufferinfo;

		channelinfo.isInput = ASIOFalse;
		channelinfo.channel = i + inputChannels;

		bufferinfo.isInput = ASIOFalse;
		bufferinfo.channelNum = i + inputChannels;

		iasio->getChannelInfo(&channelinfo);

		channelInfos.push_back(channelinfo);
		bufferInfos.push_back(bufferinfo);
	}
	


	

	iasio->getSampleRate(&sampleRate);


	ASIOBufferInfo* buffers = new ASIOBufferInfo[int(inputChannels + outputChannels)];
	std::copy(bufferInfos.begin(), bufferInfos.end(), buffers);

	ASIOCallbacks callback;
	callback.bufferSwitch = nullptr;


	iasio->createBuffers(buffers, inputChannels+outputChannels, preferredSize, &callback);







}

void Asio::CloseDriver()
{
	//DriverInfo info;
	//info.asiodrv->Release();
}

void Asio::GetInputChannels()
{
}

int Asio::GetOutputChannels()
{
	return 0;
}

int Asio::GetInputLatency()
{
	return 0;
}

int Asio::GetOutputLatency()
{
	return 0;
}

ASIOError Asio::GetSampleRate()
{
	return ASIOError();
}

ASIOError Asio::SetSampleRate()
{
	return ASIOError();
}

string Asio::GetErrorMessage()
{
	return string();
}

ASIOError Asio::StartPlayBack(int nChannel, AudioAvailable callback)
{
	return ASIOError();
}

ASIOError Asio::StopPlayBack(int nChannel)
{
	return ASIOError();
}

ASIOError Asio::StartCapture(int nChannel, AudioAvailable callback)
{
	return ASIOError();
}

ASIOError Asio::StopCapture(int nChannel)
{
	return ASIOError();
}

int Asio::getChannels()
{
	return 0;
}

int Asio::getLatencies()
{
	return 0;
}

void Asio::getDriverList()
{
	HKEY hkEnum = 0;
	HKEY hkSub = 0;
	long cr;
	DWORD index = 0;
	DWORD datatype = REG_SZ, datasize = 256;
	char keyname[MAXDRVNAMELEN];
	char	databuf[256];
	WORD	wData[100];
	CLSID	clsid;

	cr =  RegOpenKey(HKEY_LOCAL_MACHINE, ASIO_PATH, &hkEnum);

	while (cr == ERROR_SUCCESS)
	{
		if ((cr == RegEnumKey(hkEnum, index++, (LPTSTR)keyname, MAXDRVNAMELEN)) == ERROR_SUCCESS) {

			if ((cr = RegOpenKeyEx(hkEnum, (LPCTSTR)keyname, 0, KEY_READ, &hkSub)) == ERROR_SUCCESS) {

				cr = RegQueryValueEx(hkSub, COM_CLSID, 0, &datatype, (LPBYTE)databuf, &datasize);
					if (cr == ERROR_SUCCESS)
					{
						DriverInfo info;
						MultiByteToWideChar(CP_ACP, 0, (LPCSTR)databuf, -1, (LPWSTR)wData, 100);
						if ((cr = CLSIDFromString((LPOLESTR)wData, (LPCLSID)&clsid)) == S_OK) {
							memcpy(&info.clsid, &clsid, sizeof(CLSID));
						}

						datatype = REG_SZ; datasize = 256;
						cr = RegQueryValueEx(hkSub, ASIODRV_DESC, 0, &datatype, (LPBYTE)databuf, &datasize);
						if (cr == ERROR_SUCCESS) {
							strcpy_s(info.drvname, databuf);
						}
						else strcpy_s(info.drvname, keyname);

						driverInfos.push_back(info);
					}
					RegCloseKey(hkSub);
				}
			}

		}
	//应该判断下再关闭
	RegCloseKey(hkEnum);


}

void Asio::bufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
}

//这个代码要验证下是否正常生效
void Asio::sampleRateDidChange(ASIOSampleRate sRate)
{
	sampleRate = sRate;
}

long Asio::asioMessage(long selector, long value, void* message, double* opt)
{
	long ret = 0;
	switch (selector)
	{
	case kAsioSelectorSupported:
		if (value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
			ret = 1L;
		break;
	case kAsioResetRequest:
		// defer the task and perform the reset of the driver during the next "safe" situation
		// You cannot reset the driver right now, as this code is called from the driver.
		// Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
		// Afterwards you initialize the driver again.
		//asioDriverInfo.stopped;  // In this sample the processing will just stop
		ret = 1L;
		break;
	case kAsioResyncRequest:
		// This informs the application, that the driver encountered some non fatal data loss.
		// It is used for synchronization purposes of different media.
		// Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
		// Windows Multimedia system, which could loose data because the Mutex was hold too long
		// by another thread.
		// However a driver can issue it in other situations, too.
		ret = 1L;
		break;
	case kAsioLatenciesChanged:
		// This will inform the host application that the drivers were latencies changed.
		// Beware, it this does not mean that the buffer sizes have changed!
		// You might need to update internal delay data.
		ret = 1L;
		break;
	case kAsioEngineVersion:
		// return the supported ASIO version of the host application
		// If a host applications does not implement this selector, ASIO 1.0 is assumed
		// by the driver
		ret = 2L;
		break;
	case kAsioSupportsTimeInfo:
		// informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
		// is supported.
		// For compatibility with ASIO 1.0 drivers the host application should always support
		// the "old" bufferSwitch method, too.
		ret = 1;
		break;
	case kAsioSupportsTimeCode:
		// informs the driver wether application is interested in time code info.
		// If an application does not need to know about time code, the driver has less work
		// to do.
		ret = 0;
		break;
	}
	return ret;
}

ASIOTime* Asio::bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess)
{
	return nullptr;
}

