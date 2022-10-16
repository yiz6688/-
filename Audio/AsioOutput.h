#pragma once
#include "Asio/asio.h"
#include <streambuf>

struct AsioOutput
{
	long channelNum;			// on input:  channel index
	void* buffers[2];			// on output: double buffer addresses
	ASIOBool isActive;		// on exit
	long channelGroup;		// dto
	ASIOSampleType type;	// dto
	char name[32];			// dto

	bool isRuning;

	std::ostream* buf;
};