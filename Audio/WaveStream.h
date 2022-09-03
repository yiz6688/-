#pragma once
#include<iostream>
#include"WaveFormat.h"

namespace Wave
{

	class WaveStream : public std::iostream
	{
	public:
		
		~WaveStream() {};
		
		void Skip(int seconds)
		{


		 }

	public:
		WaveFormat waveformat;

		

	};

}
