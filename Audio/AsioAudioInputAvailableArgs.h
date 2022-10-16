#pragma once
#include "Asio/asio.h"
#include<vector>
using std::vector;
class AsioAudioInputAvailableArgs
{

public:
	AsioAudioInputAvailableArgs(vector<void*>&Inputs, int samplesPerBuffer, ASIOSampleType asioSampleType);

	int GetAsInterleavedSamples(vector<double>& samples);

	int GetASInterleaveSamples(vector<double>& SampleVec);


private:
	//void **InputBuffers;

	vector<void*> &InputBuffers;

	int SamplesPerBuffer;

	ASIOSampleType AsioSampleType;



};