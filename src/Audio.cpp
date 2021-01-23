#include "Audio.h"
#include "System.h"
#include "File.h"

#pragma comment(lib, "winmm.lib")

double Audio::volume = 1.0;
DATA_HEADER Audio::audioData;
FMT_HEADER Audio::formatStuff;
HWAVEOUT Audio::waveOutHandle;

int Audio::loc = 0;

std::thread Audio::audioThread;
bool Audio::playing = false;
bool Audio::running = false;

Audio::Audio()
{

}

Audio::~Audio()
{
    
}

void Audio::init()
{
    /*
    loadWave("Assets/TetrisTheme.wav");

    WAVEFORMATEX wfx;
    wfx.cbSize = 0;
    wfx.nSamplesPerSec = Audio::formatStuff.sampleRate;
    wfx.wBitsPerSample = Audio::formatStuff.bitsPerSample;
    wfx.wFormatTag = Audio::formatStuff.audioFormat;
    wfx.nChannels = Audio::formatStuff.numChannels;
    wfx.nAvgBytesPerSec = Audio::formatStuff.byteRate;
    wfx.nBlockAlign = Audio::formatStuff.blockAlign;
    
    MMRESULT result = waveOutOpen(&waveOutHandle, WAVE_MAPPER, &wfx, NULL, NULL, CALLBACK_NULL);
    if(result == MMSYSERR_NOERROR)
    {
        Audio::running = true;
        Audio::playing = false;

        Audio::audioThread = std::thread(Audio::submitData);
    }
    */
}

void Audio::dispose()
{
    stopAudio();
    /*
    playing = false;
    running = false;

    if(Audio::audioThread.joinable())
        Audio::audioThread.join();
    
    waveOutClose(waveOutHandle);
    */
}

void Audio::playAudio()
{
    PlaySoundW(L"Assets/TetrisTheme.wav", NULL, SND_ASYNC | SND_LOOP);
    //playing = true;
}

void Audio::stopAudio()
{
    PlaySoundW(NULL, NULL, NULL);
    //playing = false;
}

void Audio::setVolume(double value)
{
    volume = System::clamp<double>(value, 0.0, 1.0);
}

double Audio::getVolume()
{
    return volume;
}

void Audio::submitData()
{
    WAVEHDR header;
    ZeroMemory(&header, sizeof(WAVEHDR));
    header.dwBufferLength = ((Audio::formatStuff.bitsPerSample/8) * Audio::formatStuff.numChannels * Audio::formatStuff.sampleRate) / 1;
    header.dwFlags = 0;
    header.dwLoops = 0;

    int index = loc;
    unsigned char* lpData = new unsigned char[header.dwBufferLength];

    while(running)
    {
        if(playing)
        {
            for(int i=0; i<header.dwBufferLength; i+=2)
            {
                if(index>Audio::audioData.data.size())
                {
                    index = 0;
                }
                short v = (Audio::audioData.data[index+1]<<8)  + Audio::audioData.data[index];
                v = (short) v*Audio::getVolume();
                lpData[i+1] = (v>>8) & 0xFF;
                lpData[i] = (v) & 0xFF;
                
                index+=2;
            }
            
            header.lpData = (LPSTR)lpData;

            waveOutPrepareHeader(Audio::waveOutHandle, &header, sizeof(WAVEHDR));
            MMRESULT wResult = waveOutWrite(Audio::waveOutHandle, &header, sizeof(WAVEHDR));

            if(wResult != 0)
            {
                waveOutUnprepareHeader(Audio::waveOutHandle, &header, sizeof(WAVEHDR));
                //error has occured
                playing = false;
                MessageBeep(NULL);
            }

            Sleep(10);
            do {
                
            } while (!(header.dwFlags & WHDR_DONE));
            waveOutUnprepareHeader(Audio::waveOutHandle, &header, sizeof(WAVEHDR));

        }

    }

}

void Audio::loadWave(std::string filename)
{
    File f = File(filename);
    std::vector<unsigned char> bytes = f.getFileBytes();
    f.close();

    int index = 0;

    //RIFF HEADER
    RIFF_HEADER rHeader;
    rHeader.chunkID = bytes[index];
        rHeader.chunkID += bytes[index+1];
            rHeader.chunkID += bytes[index+2];
                rHeader.chunkID += bytes[index+3];
    index += 4;

    rHeader.chunkSize = ((int)bytes[index]) + ((int)bytes[index+1]<<8) + ((int)bytes[index+2]<<16) + ((int)bytes[index+3]<<24);
    index += 4;

    rHeader.format = bytes[index];
        rHeader.format += bytes[index+1];
            rHeader.format += bytes[index+2];
                rHeader.format += bytes[index+3];
    index += 4;

    if(rHeader.chunkID != "RIFF")
    {
        return;
    }
    if(rHeader.format != "WAVE")
    {
        return;
    }

    //FMT HEADER
    
    Audio::formatStuff.subchunkID = bytes[index];
        Audio::formatStuff.subchunkID += bytes[index+1];
            Audio::formatStuff.subchunkID += bytes[index+2];
                Audio::formatStuff.subchunkID += bytes[index+3];
    index += 4;

    Audio::formatStuff.subchunkSize = ((int)bytes[index]) + ((int)bytes[index+1]<<8) + ((int)bytes[index+2]<<16) + ((int)bytes[index+3]<<24);
    index += 4;

    if(Audio::formatStuff.subchunkID != "fmt ")
    {
        return;
    }

    Audio::formatStuff.audioFormat = ((int)bytes[index]) + ((int)bytes[index+1]<<8);
    index+=2;
    Audio::formatStuff.numChannels = ((int)bytes[index]) + ((int)bytes[index+1]<<8);
    index+=2;
    Audio::formatStuff.sampleRate = ((int)bytes[index]) + ((int)bytes[index+1]<<8) + ((int)bytes[index+2]<<16) + ((int)bytes[index+3]<<24);
    index+=4;
    Audio::formatStuff.byteRate = ((int)bytes[index]) + ((int)bytes[index+1]<<8) + ((int)bytes[index+2]<<16) + ((int)bytes[index+3]<<24);
    index+=4;
    Audio::formatStuff.blockAlign = ((int)bytes[index]) + ((int)bytes[index+1]<<8);
    index+=2;
    Audio::formatStuff.bitsPerSample = ((int)bytes[index]) + ((int)bytes[index+1]<<8);
    index+=2;

    Audio::audioData.subchunkID = bytes[index];
        Audio::audioData.subchunkID += bytes[index+1];
            Audio::audioData.subchunkID += bytes[index+2];
                Audio::audioData.subchunkID += bytes[index+3];
    index += 4;

    Audio::audioData.subchunkSize = ((int)bytes[index]) + ((int)bytes[index+1]<<8) + ((int)bytes[index+2]<<16) + ((int)bytes[index+3]<<24);
    index += 4;

    if(Audio::audioData.subchunkID != "data")
    {
        return;
    }

    for(int i=0; i<Audio::audioData.subchunkSize; i++)
    {
        Audio::audioData.data.push_back(bytes[index+i]);
    }
}