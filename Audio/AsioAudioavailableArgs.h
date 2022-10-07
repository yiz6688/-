#pragma once
#include "Asio/asio.h"

class AsioAudioAvailableArgs
{

public:
	AsioAudioAvailableArgs(void* inputBuffers[], void* outputBuffers[], int samplesPerBuffer, ASIOSampleType asioSampleType);

	int GetAsInterleavedSamples(float* samples, int bufferLen);


	float* GetASInterleaveSamples();


private:
	void **InputBuffers;

	void **OutpubBuffers;

	int SamplesPerBuffer;

	ASIOSampleType AsioSampleType;



};