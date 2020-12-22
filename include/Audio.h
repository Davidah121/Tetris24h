#pragma once
#include <Windows.h>
#include <vector>

struct Note
{
    float frequency;
    int time;
};

class Audio
{
public:
    Audio();
    ~Audio();

    void playAudio();

    static const
private:
    std::vector<Note> musicNotes;
};