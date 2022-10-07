#pragma once
#include<string>
using namespace std;
class RiffChunk
{


public:

	RiffChunk(int identifier, int length, long streamPosition)
	{
		Identifier = identifier;
		Length = length;
		StreamPosition = streamPosition;

	}

	long  getStreamPosition()
	{
		return StreamPosition;
	}

	int getLength()
	{
		return Length;
	}

private:
	int Identifier;

	string IdentifierAsString;

	int Length;

	long StreamPosition;




};