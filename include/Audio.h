#pragma once
#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

struct RIFF_HEADER
{
    std::string chunkID;
    int chunkSize;
    std::string format;
};

struct FMT_HEADER
{
    std::string subchunkID;
    int subchunkSize;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct DATA_HEADER
{
    std::string subchunkID;
    int subchunkSize;
    std::vector<unsigned char> data;
};

struct audioHeaderThing
{
    WAVEHDR header;
    bool used;
};

class Audio
{
public:
    Audio();
    ~Audio();

    static void init();

    static void playAudio();
    static void stopAudio();
    static void setVolume(double value);
    static double getVolume();
    
    static void dispose();
private:
    static void CALLBACK waveCallBack(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
    static void loadWave(std::string filename);

    static void submitData(LPSTR buffer);
    static void prepareData();

    static bool getRunning();
    static void setRunning(bool v);
    
    static bool getPlaying();

    static double volume;
    static DATA_HEADER audioData;
    static HWAVEOUT waveOutHandle;
    static FMT_HEADER formatStuff;

    static unsigned char* buffer;

    static int bufferLen;
    static int loc;
    static int buffers;

    static audioHeaderThing headers[4];

    static int callBackInc;

    static std::thread audioThread;
    static std::mutex audioMutex;
    static bool running;
    static bool playing;
};