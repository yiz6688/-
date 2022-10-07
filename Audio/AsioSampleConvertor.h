#pragma once
#include "WaveFormat.h"
#include "WaveFormatExtraData.h"
#include"Asio/asio.h"
using namespace Wave;
typedef void(*SampleConvertor)(void* inputInterleaveBuffer, void* asioOutputBuffer[], int nChannel, int nSamples);

class AsioSamopleConvertor
{

public:

	static SampleConvertor SelectSampleConvertor(WaveFormat waveFormat, ASIOSampleType asioType);

public:
	//16λת32λ
	static void ShortToInt2Channels(void* inputInterleaveBuffer, void*asioOutputBuffers[2], int nChannel, int nSamples);

	static void ShortToIntGeneric(void* inputInterleaveBuffer, void *asioOutputBuffers[2], int nChannel, int nSamples);

	static void FloatToInt2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void FloatToIntGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void ShortToShort2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void ShortToShortGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void FloatToShort2Channels(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void FloatToShortGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void FloatTo24LSBGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static void FloatToFloatGeneric(void* inputInterleaveBuffer, void* asioOutputBuffers[2], int nChannel, int nSamples);

	static int clamTo24Bit(double sampleValue)
	{
		sampleValue = (sampleValue < -1.0) ? -1.0 : (sampleValue > 1.0) ? 1.0 : sampleValue;
		return (int)(sampleValue * 8388607.0);
	}

	static int clamToInt(double sampleValue)
	{
		sampleValue = (sampleValue < -1.0) ? -1.0 : (sampleValue > 1.0) ? 1.0 : sampleValue;
		return (int)(sampleValue * 2147483647.0);
	}

	static int clamToShort(double sampleValue)
	{
		sampleValue = (sampleValue < -1.0) ? -1.0 : (sampleValue > 1.0) ? 1.0 : sampleValue;
		return (int)(sampleValue * 32767.0);
	}
};