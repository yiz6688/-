#pragma once
#include"WaveFormat.h"
#include"RiffChunk.h"
#include<stdexcept>
#include"ChunkIdentifier.h"
#include<streambuf>
#include<vector>
#include<math.h>
#include<algorithm>
using namespace Wave;


class WaveFileChunkReader
{
public:
	WaveFileChunkReader()
	{
		storeAllChunks = true;

		strictMode = false;
	}


	void ReadWaveHeader(streambuf& buf)
	{

		//�Ȼ�ȡ��ǰλ��
		streampos curpos = buf.pubseekoff(0, ios_base::cur);

		//���ĳ���
		streampos size = buf.pubseekoff(0, ios_base::end);

		buf.pubseekpos(curpos);   //�ָ���ԭ����λ�á�


		this->dataChunkPosition = -1;
		//this->waveFormat = nullptr;

		this->dataChunkLength = 0;

		BinaryReader br(&buf);

		this->riffSize = br.ReadInt32();
		if (br.ReadInt32() != ChunkIdentifier::ChunkIdentifierToInt32("WAVE"))
		{
			throw std::exception("Not a WAVE file - no WAVE header");
		}
		
		if (isRf64)
		{
			//ReadDs64Chunk(br);
		}
		int dataChunkId = ChunkIdentifier::ChunkIdentifierToInt32("data");
		int formatChunkId = ChunkIdentifier::ChunkIdentifierToInt32("fmt ");

		



		long stopPosition = std::min<long>(riffSize + 8, size);  //ǰ8���ֽڲ���riffsize �ڡ�

		streampos Position;

		while ( (Position = buf.pubseekoff(0, ios_base::cur) ) <= stopPosition - 8)
		{
			int chunkIdentifier = br.ReadInt32();
			int chunkLength = br.ReadInt32();   //read uint;  �ٷ���
			if (chunkIdentifier == dataChunkId)
			{
				dataChunkPosition = Position;

				if (!isRf64)
				{
					dataChunkLength = chunkLength;
				}

				//������������λ��
				Position += chunkLength;
				buf.pubseekpos(Position);

			}
			else if (chunkIdentifier == formatChunkId)
			{
				if (chunkLength > INT32_MAX)
				{
					throw new invalid_argument("Format chunk length must be between 0 and intmax");
					waveFormat = WaveFormat::FromFormatChunk(br, int(chunkLength));
				}
			}
			else
			{
				if (chunkLength > size - Position)
				{
					if (this->strictMode)
					{
						//��ӡ������Ϣ
					}
					break;
				}
				if (this->storeAllChunks)
				{
					if (chunkLength > INT32_MAX)
					{
						throw std::invalid_argument("RiffChunk chunk length must be between 0 and");
						riffChunks.push_back(GetRiffChunk(buf, chunkIdentifier, (int)chunkLength));
					}
				}

				//������������λ��
				Position += chunkLength;
				buf.pubseekpos(Position);
			}
			
		}
		//���ｫ���������� Ҫ����Ϊ���ж�
		/*if (waveFormat == null)
		{
			throw std::invalid_argument("Invalid WAV file - No fmt chunk found");
		}*/

		if (dataChunkPosition == -1)
		{
			throw std::invalid_argument("Invalid WAV file - No data chunk found");
		}


	}



	RiffChunk GetRiffChunk(streambuf &buf, int chunkIdentifier, int chunkLength)
	{
		//��ǰ����λ��
		streampos Position = buf.pubseekoff(0, ios_base::cur);
		return RiffChunk(chunkIdentifier, chunkLength, Position);
	}


private:
	void ReadRiffHeader(BinaryReader& br)
	{
		int header = br.ReadInt32();
		if (header == ChunkIdentifier::ChunkIdentifierToInt32("RF64"))
		{
			this->isRf64 = true;
		}
		else if (header != ChunkIdentifier::ChunkIdentifierToInt32("RIFF"))
		{
			throw std::exception("Not a WAVE file - no RIFF header");
		}

	}



private:
	WaveFormat waveFormat;

	long dataChunkPosition;

	long dataChunkLength;

	vector<RiffChunk> riffChunks;

	bool strictMode;

	bool isRf64;

	bool storeAllChunks;

	long riffSize;



};
