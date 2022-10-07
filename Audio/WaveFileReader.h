#pragma once
#include"types.h"
#include<fstream>
#include"WaveStream.h"
#include "WaveFileChunkReader.h"
#include "BitConverter.h"


namespace Wave
{
	class WaveFileReader 
	{

	public:

		WaveFileReader(istream* inputStream)
		{	
			waveStream = inputStream;
			isNew = false;
			WaveFileChunkReader* chunkReader = new WaveFileChunkReader();

			chunkReader->ReadWaveHeader(*waveStream->rdbuf());

			waveFormat = chunkReader->GetWaveFormat();
			dataPosition = 0;
			dataChunkLength = 0;
			

		}

		WaveFileReader(string waveFile)
		{
			//注意这里一定要加上2进制打开，否则后面的获取文件指针位置，移动文件指针位置会不准确
			waveStream  =  new ifstream(waveFile, std::ios::binary);


			isNew = true;

			WaveFileChunkReader* chunkReader = new WaveFileChunkReader();

			chunkReader->ReadWaveHeader(*waveStream->rdbuf());

			waveFormat = chunkReader->GetWaveFormat();
			dataPosition = 0;
			dataChunkLength = 0;
		}

		~WaveFileReader()
		{
			if (isNew)
			{
				delete waveStream;
			}
		}


		void GetChunkData(RiffChunk chunk)
		{
			long oldPosition = waveStream->tellg();
			long pos = chunk.getStreamPosition();
			
			int len = chunk.getLength();
			char* data = new char[len];

			waveStream->read(data, len);

			waveStream->seekg(oldPosition);
		}


		int Read(char* array, int arraylen, int offset, int count)
		{
			if (count % waveFormat->blockAlign != 0)
			{
				throw new std::exception();
			}

			if (dataPosition + count > dataChunkLength)
			{
				count = (int)(dataChunkLength - dataPosition);
			}
			//get会检查是否有终止符，一般用来读取文本， read不会，一般用来读取二进制
			 waveStream->read(array, arraylen);

			 return waveStream->gcount();
		}


		int ReadNextSampleFrame(vector<float> vec)
		{

			switch (waveFormat->waveFormatTag)
			{
				case WaveFormatEncoding::Pcm:
				case WaveFormatEncoding::IeeeFloat:
				case WaveFormatEncoding::Extensible:
					break;
				default:
					throw new exception("not support");
			}

			vec.clear();
			int bytesToRead = waveFormat->channels * (waveFormat->bitsPerSample / 8.0);
			char* raw = new char[bytesToRead];   //将来替换成vector 或者stringbuf

			 int bytesRead =  this->Read(raw, bytesToRead, 0, bytesToRead);

			 if (bytesRead == 0) return 0;
			 if (bytesRead < bytesToRead) throw new exception("异常");

			 int offset = 0;

			 for (int channel = 0; channel < waveFormat->channels; channel++)
			 {
				 if (waveFormat->bitsPerSample == 16)
				 {
					 float ret = BitConverter::ToInt16(raw, bytesToRead, offset) / numeric_limits<short>::max();
					 vec.push_back(ret);
					 offset += 2;
				 }
				 else  if (waveFormat->bitsPerSample == 24)
				 {
					 float ret = ((raw[offset + 2] << 16) | (raw[offset + 1] << 8) | (raw[offset])) / 8388608.0f;

					 vec.push_back(ret);
					 offset += 3;
				 }
				 else if (waveFormat->bitsPerSample == 32 && waveFormat->waveFormatTag == WaveFormatEncoding::IeeeFloat)
				 {
					 float ret = BitConverter::ToSingle(raw, bytesToRead, offset);

					 vec.push_back(ret);
					 offset += 4;
				 }
				 else if (waveFormat->bitsPerSample == 32)
				 {
					 float ret  = BitConverter::ToInt32(raw, bytesToRead, offset) / (numeric_limits<int>::max() +1.0f);
					 vec.push_back(ret);
					 offset += 4;
				 }
				 else
				 {
					 throw new exception("Unsupported bit depth");
				 }
			 }



			delete[] raw;
			return 0;
		}


	private:
		

	private:
		
		bool isNew = false;

		istream* waveStream;
		
		WaveFormat *waveFormat;

		long dataPosition;

		long dataChunkLength;




	};

}