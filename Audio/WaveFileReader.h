#pragma once
#include<fstream>
#include"WaveStream.h"

namespace Wave
{
	class WaveFileReader 
	{


	public:

		WaveFileReader(istream* inputStream)
		{
			waveStream = inputStream;
		}

		WaveFileReader(string waveFile)
		{
			waveStream  =  new ifstream(waveFile);
		}

	private:
		

	private:
		
		istream* waveStream;




	};

}