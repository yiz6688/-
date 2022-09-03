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
			ReadDs64Chunk(br);
		}
		int dataChunkId = ChunkIdentifier::ChunkIdentifierToInt32("data");
		int formatChunkId = ChunkIdentifier::ChunkIdentifierToInt32("fmt ");

		//流的长度
		streampos size =  buf.pubseekoff(0, ios_base::end);
		
		buf.pubseekpos(0, ios_base::beg);   //恢复到头位置。



		long stopPosition = std::min<long>(riffSize + 8, size);  //****
		while (stream.pubseekpos() <= stopPosition - 8)
		{
			int chunkIdentifier = br.ReadInt32();
			int chunkLength = br.ReadInt32();   //read uint;  少方法
			if (chunkIdentifier == dataChunkId)
			{
				dataChunkPosition = stream.Position;

				if (!isRf64)
				{
					dataChunkLength = chunkLength;
				}

				streamPosition += chunkLength;

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
				if (chunkLength > stream.Length - stream.Position)
				{
					if (this->strictMode)
					{
						//打印错误信息
					}
					break;
				}
				if (this->storeAllChunks)
				{
					if (chunkLength > INT32_MAX)
					{
						throw std::invalid_argument("RiffChunk chunk length must be between 0 and");
						riffChunks.push_back(GetRiffChunk(stream, chunkIdentifier, (int)chunkLength));
					}
				}
				stream.Position += chunkLength;
			}
			
		}

		if (waveFormat == null)
		{
			throw std::invalid_argument("Invalid WAV file - No fmt chunk found");
		}

		if (dataChunkPosition == -1)
		{
			throw std::invalid_argument("Invalid WAV file - No data chunk found");
		}


	}



	RiffChunk GetRiffChunk(istream stream, int chunkIdentifier, int chunkLength)
	{
		return RiffChunk(chunkIdentifier, chunkLength, stream.Position);
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
