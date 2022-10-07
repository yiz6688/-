#pragma once

#include "WaveFormat.h"

namespace Wave
{
	

	class WaveFormatExtraData :  public WaveFormat
	{
	private:
		char* extraData;

	public:
		void ReadExtraData(BinaryReader &br)
		{
			if (this->extraSize > 0)
			{
				br.Read(extraData, 0, this->extraSize);
			}
		}

	public:

		WaveFormatExtraData()
		{

		}

		WaveFormatExtraData(std::istream &is)
		{

		}

	
	};




}