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


IASIO* AsioDriver::iasio;

long AsioDriver::inputChannels;

long AsioDriver::outputChannels;

long AsioDriver::minSize;

long AsioDriver::maxSize;

long AsioDriver::preferredSize;

long AsioDriver::granularity;

ASIOSampleRate AsioDriver::sampleRate;

long AsioDriver::inputLatency;

long AsioDriver::outputLatency;

long AsioDriver::inputBuffers;

long AsioDriver::outputBuffers;

vector<ASIOBufferInfo> AsioDriver::bufferInfos;

vector<ASIOChannelInfo> AsioDriver::channelInfos;

vector<DriverInfo> AsioDriver::driverInfos;


vector<AsioInput> AsioDriver::AsioInputs;

vector<AsioOutput> AsioDriver::AsioOutputs;

mutex AsioDriver::m;

void AsioDriver::Init()
{
	getDriverList();

	if (driverInfos.empty() == false) CoInitialize(0);	// 初始化COM
}

void AsioDriver::OpenDriver(int drvID)
{
	if (drvID > driverInfos.size())
	{
		//return DRVERR_DEVICE_NOT_FOUND;
	}

	DriverInfo info = driverInfos.at(drvID);   //获取对应的驱动信息，这里没有进行判断处理


	HRESULT res;

	if (info.asiodrv == nullptr)
	{
		res = CoCreateInstance(info.clsid, 0, CLSCTX_INPROC_SERVER, info.clsid, (LPVOID*)(&iasio));

		if (res == S_OK)
		{
			info.asiodrv = iasio;
		}
	}
	else
	{
		//return DRVERR_DEVICE_ALREADY_OPEN;
	}
	
	void* handle = nullptr;
	iasio->init(handle);   //初始化驱动
	ASIOError ret;
	char name[32];

	char errMsg[256];

	iasio->getDriverName(name);

	ret = iasio->getBufferSize(&minSize , &maxSize, &preferredSize , &granularity);   //获取缓冲区

	if (ret != ASE_OK)
	{
		iasio->getErrorMessage(errMsg);
		cout << errMsg << endl;
		return;
	}

	ret = iasio->getLatencies(&inputLatency, &outputLatency);   //获取延时

	if (ret != ASE_OK)
	{
		iasio->getErrorMessage(errMsg);
		cout << errMsg << endl;
		return;
	}

	ret = iasio->getChannels(&inputChannels, &outputChannels);   //获取通道数

	if (ret != ASE_OK)
	{
		iasio->getErrorMessage(errMsg);
		cout << errMsg << endl;
		return;
	}
	

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
		channelinfo.channel = i ;

		bufferinfo.isInput = ASIOFalse;
		bufferinfo.channelNum = i ;

		iasio->getChannelInfo(&channelinfo);

		channelInfos.push_back(channelinfo);
		bufferInfos.push_back(bufferinfo);
	}
	


	iasio->getSampleRate(&sampleRate);

	int bufferlen = inputChannels + outputChannels;

	ASIOBufferInfo* buffers = new ASIOBufferInfo[bufferInfos.size()];
	std::copy(bufferInfos.begin(), bufferInfos.end(), buffers);

	ASIOCallbacks callback;
	callback.bufferSwitch = bufferSwitch;
	callback.asioMessage = asioMessage;
	callback.bufferSwitchTimeInfo = bufferSwitchTimeInfo;
	callback.sampleRateDidChange = sampleRateDidChange;


	iasio->createBuffers(buffers, inputChannels+outputChannels, preferredSize, &callback);



}

void AsioDriver::CloseDriver()
{
	//DriverInfo info;
	//info.asiodrv->Release();
}

void AsioDriver::GetInputChannels()
{
}

int AsioDriver::GetOutputChannels()
{
	return 0;
}

int AsioDriver::GetInputLatency()
{
	return 0;
}

int AsioDriver::GetOutputLatency()
{
	return 0;
}

ASIOError AsioDriver::GetSampleRate()
{
	return ASIOError();
}

ASIOError AsioDriver::SetSampleRate()
{
	return ASIOError();
}

string AsioDriver::GetErrorMessage()
{
	return string();
}

ASIOError AsioDriver::StartPlayBack(int channelOffset, int channenNum, AudioInputAvailable callback)
{



	return ASIOError();
}

ASIOError AsioDriver::StopPlayBack(int nChannel)
{
	return ASIOError();
}

ASIOError AsioDriver::StartCapture(int nChannel, AudioOutputAvailable callback)
{
	return ASIOError();
}

ASIOError AsioDriver::StopCapture(int nChannel)
{
	return ASIOError();
}

int AsioDriver::getChannels()
{
	return 0;
}

int AsioDriver::getLatencies()
{
	return 0;
}

void AsioDriver::getDriverList()
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
		if ((cr = RegEnumKey(hkEnum, index++, (LPTSTR)keyname, MAXDRVNAMELEN)) == ERROR_SUCCESS) {

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
							strcpy_s(info.drvname, MAXDRVNAMELEN, databuf);
						}
						else strcpy_s(info.drvname,MAXDRVNAMELEN, keyname);

						driverInfos.push_back(info);
					}
					RegCloseKey(hkSub);
				}
			}

		}
	//应该判断下再关闭
	RegCloseKey(hkEnum);


}
/// <summary>
/// 回调函数，该函数是返回缓冲的索引，0 或 1
/// 
/// </summary>
/// <param name="doubleBufferIndex"></param>
/// <param name="directProcess"></param>
void AsioDriver::bufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{

	vector<void*> buffers;
	for (AsioInput input : AsioInputs)
	{
		if (input.isRuning)
		{
			buffers.clear();
			int channelCount = input.channelCount;
		
			
			for (int i = 0; i < channelCount; i++)
			{
				void* inputbuffer = AsioInputs[i].buffers[doubleBufferIndex];
				buffers.push_back(inputbuffer);   //缓冲区集合
			}
			AsioAudioInputAvailableArgs args(buffers, preferredSize, input.type);
			input.callback(args);
		}
		
		

	}






}

//这个代码要验证下是否正常生效
void AsioDriver::sampleRateDidChange(ASIOSampleRate sRate)
{
	sampleRate = sRate;
}

long AsioDriver::asioMessage(long selector, long value, void* message, double* opt)
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

ASIOTime* AsioDriver::bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess)
{
	return nullptr;
}

bool AsioDriver::hasRuning()
{
	m.lock();

	for (AsioInput input : AsioInputs)
	{
		if (input.isRuning)
		{
			return true;
		}
	}

	for (AsioOutput output : AsioOutputs)
	{
		if (output.isRuning)
		{
			return true;
		}
	}

	m.unlock();

	return false;

}

