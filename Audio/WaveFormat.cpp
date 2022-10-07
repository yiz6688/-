#include"WaveFormat.h"

#include"WaveFormatExtraData.h"

namespace Wave
{

    /// <summary>
         /// Reads in a WaveFormat (with extra data) from a fmt chunk (chunk identifier and
         /// length should already have been read)
         /// </summary>
         /// <param name="br">Binary reader</param>
         /// <param name="formatChunkLength">Format chunk length</param>
         /// <returns>A WaveFormatExtraData</returns>

    WaveFormat WaveFormat::FromFormatChunk(BinaryReader br, int formatChunkLength)
    {
        WaveFormatExtraData waveFormat;
        waveFormat.ReadWaveFormat(br, formatChunkLength);
        waveFormat.ReadExtraData(br);

        return waveFormat;
    }

}

