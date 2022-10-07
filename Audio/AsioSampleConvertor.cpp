#include "AsioSampleConvertor.h"


SampleConvertor AsioSamopleConvertor::SelectSampleConvertor(WaveFormat waveFormat, ASIOSampleType asioType)
{
	SampleConvertor convertor = nullptr;
	bool is2Channel = waveFormat.channels == 2;

	switch (asioType)
	{
	case ASIOSTInt32LSB:
		switch (waveFormat.bitsPerSample)
		{
		case 16:
			convertor = (is2Channel) ? ShortToInt2Channels : ShortToIntGeneric;
			break;
		case 32:
			convertor = (is2Channel) ? FloatToInt2Channels : FloatToIntGeneric;
		}
		break;
	case ASIOSTInt16LSB:
		switch (waveFormat.bitsPerSample)
		{
		case 16:
			convertor = (is2Channel) ? ShortToShort2Channels : ShortToShortGeneric;
			break;
		case 32:
			convertor = (is2Channel) ? FloatToShort2Channels : FloatToShortGeneric;
		}
		break;
	case ASIOSTInt24LSB:
		switch (waveFormat.bitsPerSample)
		{
		case 16:
			convertor = nullptr; //抛出异常
			break;
		case 32:
			convertor = FloatTo24LSBGeneric;
		}
		break;
	case ASIOSTFloat32LSB:
		switch (waveFormat.bitsPerSample)
		{
		case 16:
			convertor = nullptr; //抛出异常
			break;
		case 32:
			convertor = FloatToFloatGeneric;
		}
		break;
	default:
		//抛出异常
		break;
	}

	return convertor;
}

void AsioSamopleConvertor::ShortToInt2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	short* inputSamples = static_cast<short*>(inputInterleaveBuffer);
	short* leftSamples = static_cast<short*>(asioOutputBuffers[0]);
	short* rightSamples = static_cast<short*>(asioOutputBuffers[1]);

	//check
	leftSamples++;
	rightSamples++;

	for (int i = 0; i < nSamples; i++)
	{
		*leftSamples = inputSamples[0];
		*rightSamples = inputSamples[1];

		inputSamples += 2;
		//check +2 应该是int抓short的一个过程
		leftSamples += 2;
		rightSamples += 2;
	}


}

void AsioSamopleConvertor::ShortToIntGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{

	short* inputSamples = static_cast<short*>(inputInterleaveBuffer);

	short** samples = new short*[nChannel];

	for (int i = 0; i < nChannel; i++)
	{
		samples[i] = static_cast<short*>(asioOutputBuffers[i]);
		samples[i]++;   //check, 这里可能涉及int转short的高低位置
	}

	for (int i = 0; i < nSamples; i++)
	{
		for (int j = 0; j < nChannel; j++)
		{
			*samples[j] = *inputSamples++;
			samples[j] += 2;//check
		}
	}

	delete[] samples;

}

void AsioSamopleConvertor::FloatToInt2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	float* inputSamples = static_cast<float*>(inputInterleaveBuffer);
	short* leftSamples = static_cast<short*>(asioOutputBuffers[0]);
	short* rightSamples = static_cast<short*>(asioOutputBuffers[1]);


	for (int i = 0; i < nSamples; i++)
	{
		*leftSamples++ = clamToInt(inputSamples[0]);
		*rightSamples++ = clamToInt(inputSamples[1]);
		inputSamples += 2;
	}
}

void AsioSamopleConvertor::FloatToIntGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	float* inputSamples = static_cast<float*>(inputInterleaveBuffer);

	short** samples = new short* [nChannel];

	for (int i = 0; i < nChannel; i++)
	{
		samples[i] = static_cast<short*>(asioOutputBuffers[i]);
	}

	for (int i = 0; i < nSamples; i++)
	{
		for (int j = 0; j < nChannel; j++)
		{
			*samples[j] = clamToInt(*inputSamples++);
		}
	}

	delete[] samples;
}

void AsioSamopleConvertor::ShortToShort2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	short* inputSamples = static_cast<short*>(inputInterleaveBuffer);
	short* leftSamples = static_cast<short*>(asioOutputBuffers[0]);
	short* rightSamples = static_cast<short*>(asioOutputBuffers[1]);


	for (int i = 0; i < nSamples; i++)
	{
		*leftSamples++ = clamToInt(inputSamples[0]);
		*rightSamples++ = clamToInt(inputSamples[1]);
		inputSamples += 2;
	}
}

void AsioSamopleConvertor::ShortToShortGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	short* inputSamples = static_cast<short*>(inputInterleaveBuffer);

	short** samples = new short* [nChannel];

	for (int i = 0; i < nChannel; i++)
	{
		samples[i] = static_cast<short*>(asioOutputBuffers[i]);
	}

	for (int i = 0; i < nSamples; i++)
	{
		for (int j = 0; j < nChannel; j++)
		{
			*samples[j] = clamToInt(*inputSamples++);
		}
	}

	delete[] samples;

}

void AsioSamopleConvertor::FloatToShort2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	float* inputSamples = static_cast<float*>(inputInterleaveBuffer);
	short* leftSamples = static_cast<short*>(asioOutputBuffers[0]);
	short* rightSamples = static_cast<short*>(asioOutputBuffers[1]);


	for (int i = 0; i < nSamples; i++)
	{
		*leftSamples++ = clamToInt(inputSamples[0]);
		*rightSamples++ = clamToInt(inputSamples[1]);
		inputSamples += 2;
	}
}

void AsioSamopleConvertor::FloatToShortGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	float* inputSamples = static_cast<float*>(inputInterleaveBuffer);
	short** samples = new short* [nChannel];
	for (int i = 0; i < nChannel; i++)
	{
		samples[i] = static_cast<short*>(asioOutputBuffers[i]);
	}

	for (int i = 0; i < nSamples; i++)
	{
		for (int j = 0; j < nChannel; j++)
		{
			*samples[j] = clamToInt(*inputSamples++);
		}
	}

	delete[] samples;
}

void AsioSamopleConvertor::FloatTo24LSBGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	float* intputSamples = static_cast<float*>(inputInterleaveBuffer);
	char** samples = new char* [nChannel];
	for (int i = 0; i < nChannel; i++)
	{
		samples[i] = static_cast<char*>(asioOutputBuffers[i]);
	}
	for (int i = 0; i < nSamples; i++)
	{
		for (int j = 0; j < nChannel; j++)
		{
			int sample24 = clamTo24Bit(*intputSamples++);
			*(samples[j]++) = static_cast<char>(sample24);
			*(samples[j]++) = static_cast<char>(sample24 >> 8);
			*(samples[j]++) = static_cast<char>(sample24 >> 16);
		}
	}
}

void AsioSamopleConvertor::FloatToFloatGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples)
{
	float* inputSamples = static_cast<float*>(inputInterleaveBuffer);
	float** samples = new float* [nChannel];
	for (int i = 0; i < nChannel; i++)
	{
		samples[i] = (float*)asioOutputBuffers[i];
	}

	for (int i = 0; i < nSamples; i++)
	{
		for (int j = 0; j < nChannel; j++)
		{
			*(samples[j]++) = *inputSamples++;
		}
	}
}
