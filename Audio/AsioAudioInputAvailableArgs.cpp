#include"AsioAudioInputAvailableArgs.h"
#include <limits>

AsioAudioInputAvailableArgs::AsioAudioInputAvailableArgs(vector<void*>& Inputs, int samplesPerBuffer, ASIOSampleType asioSampleType):
	InputBuffers(Inputs)
{
	//InputBuffers = inputBuffers;
	SamplesPerBuffer = samplesPerBuffer;
	AsioSampleType = asioSampleType;

}

int AsioAudioInputAvailableArgs::GetAsInterleavedSamples(vector<double>& samples)
{
	int channels = InputBuffers.size(); // 通道数，根据inputBuffers确认
	if (samples.size() < SamplesPerBuffer * channels)
	{
		//抛出异常
	}
	int index = 0;
	if (AsioSampleType == ASIOSTInt32LSB)
	{
		for (int i = 0; i < SamplesPerBuffer; i++)
		{
			for (int ch = 0; ch < channels; ch++)
			{
				samples[index++] = *((int*)InputBuffers[ch] + i) / (float)std::numeric_limits<int>::max();
			}
		}
	}
	else if (AsioSampleType == ASIOSTInt16LSB)
	{
		for (int i = 0; i < SamplesPerBuffer; i++)
		{
			for (int ch = 0; ch < channels; ch++)
			{
				samples[index++] = *((short*)InputBuffers[ch] + i) / (float)std::numeric_limits<short>::max();
			}
		}

	}
	else if (AsioSampleType == ASIOSTInt24LSB)
	{
		for (int i = 0; i < SamplesPerBuffer; i++)
		{
			for (int ch = 0; ch < channels; ch++)
			{
				char* pSample = (char*)InputBuffers[ch] + i * 3;

				int sample = pSample[0] | (pSample[1] << 8) | (pSample[2] << 16);
				samples[index++] = sample / 8388608.0f;
			}
		}
	}
	else if (AsioSampleType == ASIOSTFloat32LSB)
	{
		for (int i = 0; i < SamplesPerBuffer; i++)
		{
			for (int ch = 0; ch < channels; ch++)
			{
				samples[index++] = *((float*)InputBuffers[ch] + i);
			}
		}

	}
	else
	{
		//抛出异常，暂时不支持
	}
	return 0;
}
/// <summary>
/// 返回交错的浮点数据，调用方根据调用的通道自行拆分
/// </summary>
/// <param name="SampleVec"></param>
/// <returns></returns>
int AsioAudioInputAvailableArgs::GetASInterleaveSamples(vector<double>& SampleVec)
{
	int channels = InputBuffers.size();  //根据InputBuffers 获取

	//float* samples = new float[SamplesPerBuffer * channels];
	size_t newsize = SamplesPerBuffer * channels;
	SampleVec.resize(newsize);
	GetAsInterleavedSamples(SampleVec);

	return SampleVec.size();
}
