#pragma once
#include"WaveFormatEncoding.h"
#include <stdexcept>
#include<istream>
#include<ostream>
#include "BinaryReader.h"

namespace Wave {

    class WaveFormat
    {
 
    protected:
        /// <summary>��Ƶ��ʽ����</summary>
        WaveFormatEncoding waveFormatTag;
        /// <summary>ͨ����</summary>
        short channels;
        /// <summary>������</summary>
        int sampleRate;
        /// <summary>ÿ��ƽ���ֽ���</summary>
        int averageBytesPerSecond;
        /// <summary>���С</summary>
        short blockAlign;
        /// <summary>ÿͨ��λ��</summary>
        short bitsPerSample;
        /// <summary>��չ��С</summary>
        short extraSize;

    public:
        WaveFormat() : WaveFormat(48000, 16, 2)
        {

        }

        WaveFormat(int sampleRate, int channels) : WaveFormat(sampleRate, 16, channels)
        {

        }

        WaveFormat(int rate, int bits, int channels)
        {
            if (channels < 1)
            {
                throw std::invalid_argument::invalid_argument("channel����С��1");
            }
            // minimum 16 bytes, sometimes 18 for PCM
            this->waveFormatTag = WaveFormatEncoding::Pcm;
            this->channels = (short)channels;
        
            sampleRate = rate;
            bitsPerSample = (short)bits;
            extraSize = 0;

            this->blockAlign = (short)(channels * (bits / 8));
            this->averageBytesPerSecond = this->sampleRate * this->blockAlign;
        }

        /// <summary>
        /// �����������ʱ��ȡ��������С
        /// </summary>
        /// <param name="milliseconds">The milliseconds.</param>
        /// <returns></returns>
        int ConvertLatencyToByteSize(int milliseconds)
        {
            int bytes = (int)((averageBytesPerSecond / 1000.0) * milliseconds);
            if ((bytes % blockAlign) != 0)
            {
                // Return the upper BlockAligned
                bytes = bytes + blockAlign - (bytes % blockAlign);
            }
            return bytes;
        }

        /// <summary>
          /// ����32,IEEE��������ʽ
          /// </summary>
          /// <param name="sampleRate">sample rate</param>
          /// <param name="channels">number of channels</param>
        static WaveFormat CreateIeeeFloatWaveFormat(int sampleRate, int channels)
        {
            WaveFormat wf ;
            wf.waveFormatTag = WaveFormatEncoding::IeeeFloat;
            wf.channels = (short)channels;
            wf.bitsPerSample = 32;
            wf.sampleRate = sampleRate;
            wf.blockAlign = (short)(4 * channels);
            wf.averageBytesPerSecond = sampleRate * wf.blockAlign;
            wf.extraSize = 0;
            return wf;
        }

        /// <summary>
        /// Reads in a WaveFormat (with extra data) from a fmt chunk (chunk identifier and
        /// length should already have been read)
        /// </summary>
        /// <param name="br">Binary reader</param>
        /// <param name="formatChunkLength">Format chunk length</param>
        /// <returns>A WaveFormatExtraData</returns>
        static WaveFormat FromFormatChunk(BinaryReader br, int formatChunkLength)
        {
            WaveFormatExtraData waveFormat;
            waveFormat.ReadWaveFormat(br, formatChunkLength);
            waveFormat.ReadExtraData(br);
            
            return waveFormat;
        }

         void ReadWaveFormat(BinaryReader br, int formatChunkLength)
        {
            if (formatChunkLength < 16)
            {
                throw std::invalid_argument::invalid_argument("Invalid WaveFormat Structure");
            }
            waveFormatTag = (WaveFormatEncoding)br.ReadInt16();
            sampleRate = br.ReadInt32();
            averageBytesPerSecond = br.ReadInt32();
            blockAlign = br.ReadInt16();
            bitsPerSample = br.ReadInt16();
            if (formatChunkLength > 16)
            {
                extraSize = br.ReadInt16();
                if (extraSize != formatChunkLength - 18)
                {
                    cout << "Format chunk mismatch";
                    extraSize = (short)(formatChunkLength - 18);
                }
            }
        }

         void WaveFormoat(BinaryReader br)
         {
             int formatChunkLength = br.ReadInt32();
             ReadWaveFormat(br, formatChunkLength);
         }

         string ToString()
         {
             switch (waveFormatTag)
             {
                 case WaveFormatEncoding::Pcm:
                 case WaveFormatEncoding::Extensible:
                     return "";
                 default:
                     return "other";

             }
         }

         bool operator == (const WaveFormat &fmt)
         {
             return this->waveFormatTag == fmt.waveFormatTag &&
                 this->channels == fmt.channels &&
                 this->sampleRate == fmt.sampleRate &&
                 this->blockAlign == fmt.blockAlign &&
                 this->bitsPerSample == fmt.bitsPerSample;
         }




    };

}
