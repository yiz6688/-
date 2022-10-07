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
			//ע������һ��Ҫ����2���ƴ򿪣��������Ļ�ȡ�ļ�ָ��λ�ã��ƶ��ļ�ָ��λ�û᲻׼ȷ
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
			//get�����Ƿ�����ֹ����һ��������ȡ�ı��� read���ᣬһ��������ȡ������
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
			char* raw = new char[bytesToRead];   //�����滻��vector ����stringbuf

			 int bytesRead =  this->Read(raw, bytesToRead, 0, bytesToRead);

			 if (bytesRead == 0) return 0;
			 if (bytesRead < bytesToRead) throw new exception("�쳣");

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