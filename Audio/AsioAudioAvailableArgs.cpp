#include"AsioAudioavailableArgs.h"
#include <limits>

AsioAudioAvailableArgs::AsioAudioAvailableArgs(void* inputBuffers[], void* outputBuffers[], int samplesPerBuffer, ASIOSampleType asioSampleType)
{
	InputBuffers = inputBuffers;
	OutpubBuffers = outputBuffers;
	SamplesPerBuffer = samplesPerBuffer;
	AsioSampleType = asioSampleType;

}

int AsioAudioAvailableArgs::GetAsInterleavedSamples(float* samples, int bufferLen)
{
	int channels = 2; // ͨ����������inputBuffersȷ��
	if (bufferLen < SamplesPerBuffer * channels)
	{
		//�׳��쳣
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
		//�׳��쳣����ʱ��֧��
	}
	return 0;
}

float* AsioAudioAvailableArgs::GetASInterleaveSamples()
{
	int channels = 2;  //����InputBuffers ��ȡ

	float* samples = new float[SamplesPerBuffer * channels];
	GetAsInterleavedSamples(samples, 0);

	return samples;
}
