#include "Audio.h"
#include "System.h"
#include "File.h"

#pragma comment(lib, "winmm.lib")

double Audio::volume = 0.5;
DATA_HEADER Audio::audioData;
FMT_HEADER Audio::formatStuff;
HWAVEOUT Audio::waveOutHandle;

unsigned char* Audio::buffer;
int Audio::loc = 0;

int Audio::bufferLen = 0;
int Audio::buffers = 4;

std::thread Audio::audioThread;
std::mutex Audio::audioMutex;
bool Audio::playing = false;
bool Audio::running = false;

audioHeaderThing Audio::headers[4];

int Audio::callBackInc = 0;

Audio::Audio()
{

}

Audio::~Audio()
{
    
}

void Audio::init()
{
    loadWave("Assets/TetrisTheme.wav");

    WAVEFORMATEX wfx;
    wfx.cbSize = 0;
    wfx.nSamplesPerSec = Audio::formatStuff.sampleRate;
    wfx.wBitsPerSample = Audio::formatStuff.bitsPerSample;
    wfx.wFormatTag = Audio::formatStuff.audioFormat;
    wfx.nChannels = Audio::formatStuff.numChannels;
    wfx.nAvgBytesPerSec = Audio::formatStuff.byteRate;
    wfx.nBlockAlign = Audio::formatStuff.blockAlign;
    bufferLen = 1764*5;

    buffer = new unsigned char[bufferLen*buffers];

    ZeroMemory(buffer, bufferLen*buffers);

    
    MMRESULT result = waveOutOpen(&waveOutHandle, WAVE_MAPPER, &wfx, (DWORD_PTR)Audio::waveCallBack, NULL, CALLBACK_FUNCTION);

    if(result == MMSYSERR_NOERROR)
    {
        Audio::running = true;
        Audio::playing = false;

        Audio::audioThread = std::thread(Audio::prepareData);
    }
}

void Audio::dispose()
{
    setRunning(false);
    stopAudio();

    waveOutUnprepareHeader(waveOutHandle, &(headers[loc].header), sizeof(WAVEHDR));

    if(Audio::audioThread.joinable())
        Audio::audioThread.join();
    
    waveOutClose(waveOutHandle);
}

void Audio::playAudio()
{
    //PlaySoundW(L"Assets/TetrisTheme.wav", NULL, SND_ASYNC | SND_LOOP);
    Audio::audioMutex.lock();
    playing = true;
    Audio::audioMutex.unlock();
    
}

void Audio::stopAudio()
{
    //PlaySoundW(NULL, NULL, NULL);
    Audio::audioMutex.lock();
    playing = false;
    Audio::audioMutex.unlock();
}

bool Audio::getPlaying()
{
    bool b;
    Audio::audioMutex.lock();
    b = playing;
    Audio::audioMutex.unlock();
    return b;
}

void Audio::setVolume(double value)
{
    volume = System::clamp<double>(value, 0.0, 1.0);
}

double Audio::getVolume()
{
    return volume;
}

bool Audio::getRunning()
{
    bool b;
    Audio::audioMutex.lock();
    b = running;
    Audio::audioMutex.unlock();
    return b;
}

void Audio::setRunning(bool v)
{
    Audio::audioMutex.lock();
    running = v;
    Audio::audioMutex.unlock();
}

void CALLBACK Audio::waveCallBack(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    if(uMsg == WOM_DONE)
    {
        headers[callBackInc].used = true;
        //std::cout << callBackInc;
        callBackInc++;
        if(callBackInc>=buffers)
        {
            callBackInc = 0;
        }
    }
}

void Audio::prepareData()
{
    int index = 0;
    for(int i=0; i<bufferLen*buffers; i+=2)
    {
        if(index>Audio::audioData.data.size())
        {
            index = 0;
        }
        short v = (Audio::audioData.data[index+1]<<8)  + Audio::audioData.data[index];
        v = (short) v*Audio::getVolume();
        buffer[i+1] = (v>>8) & 0xFF;
        buffer[i] = (v) & 0xFF;
        
        index+=2;
    }
    
    submitData((LPSTR)buffer);

    loc++;
    submitData((LPSTR)buffer + bufferLen);

    loc = 0;
    while(Audio::getRunning())
    {
        if(playing)
        {
            int start = bufferLen*loc;
            int end = start+bufferLen;
            for(int i=start; i<end; i+=2)
            {
                if(index>Audio::audioData.data.size())
                {
                    index = 0;
                }
                short v = (Audio::audioData.data[index+1]<<8)  + Audio::audioData.data[index];
                v = (short) v*Audio::getVolume();
                buffer[i+1] = (v>>8) & 0xFF;
                buffer[i] = (v) & 0xFF;
                
                index+=2;
            }
            
            while(headers[loc].used==false)
            {
                System::sleep(0, 1);
                if(!playing)
                {
                    break;
                }
            }

            headers[loc].used = false;

            waveOutUnprepareHeader(waveOutHandle, &(headers[(loc)].header), sizeof(WAVEHDR));
            submitData((LPSTR)buffer + (bufferLen*loc));

            loc++;
            if(loc>=buffers)
            {
                loc = 0;
            }
        }

    }
}

void Audio::submitData(LPSTR buffer)
{
    ZeroMemory(&(headers[loc].header), sizeof(WAVEHDR));
    headers[loc].header.dwBufferLength = bufferLen;
    headers[loc].header.dwFlags = 0;
    headers[loc].header.dwLoops = 0;
    headers[loc].header.lpData = (LPSTR)buffer;

    waveOutPrepareHeader(waveOutHandle, &(headers[loc].header), sizeof(WAVEHDR));
    MMRESULT wResult = waveOutWrite(Audio::waveOutHandle, &(headers[loc].header), sizeof(WAVEHDR));
    if(wResult != 0)
    {
        //error has occured
        playing = false;
        MessageBeep(NULL);
        std::cout << "ERROR: " << (int) wResult << std::endl;
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