#pragma once
#include "types.h"
class BitConverter
{
public:
	static bool IsLittleEndian;

	static float ToSingle(char* value,int valuelen, int startIndex)
	{
		return *(float*)value;
	}

	static short ToInt16(char* value, int valuelen, int startIndex)
	{
		return *(short*)value;
	}

	static int ToInt32(char* value, int valuelen, int startIndex)
	{
		return *(int*)value;
	}

};

bool BitConverter::IsLittleEndian = true;