#pragma once
#include "Asio/asio.h"
#include <streambuf>
#include <istream>
#include<vector>
#include"AsioAudioInputAvailableArgs.h"
using namespace std;

typedef void(*AudioInputAvailable)(AsioAudioInputAvailableArgs args);


struct AsioInput
{

	long channelNum;			// on input:  channel index

	int channelCount;           //Æ«ÒÆÊýÁ¿


	void* buffers[2];			// on output: double buffer addresses
	ASIOBool isActive;		// on exit
	long channelGroup;		// dto
	ASIOSampleType type;	// dto
	char name[32];			// dto

	bool isRuning;

	AudioInputAvailable  callback; //callback func;
};


