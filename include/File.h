#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class File
{
public:
    File(std::string filename);
    ~File();

    std::vector<unsigned char> getFileBytes();
    void close();

private:
    bool valid = false;
    std::fstream* fileStream;
};