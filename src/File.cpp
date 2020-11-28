#include "File.h"

File::File(std::string filename)
{
    fileStream = new std::fstream(filename, std::fstream::in | std::fstream::binary);
    if(fileStream->is_open())
        valid=true;
}

File::~File()
{
    close();
    delete fileStream;
}

void File::close()
{
    if(fileStream->is_open())
    {
        fileStream->close();
        valid=false;
    }
}

std::vector<unsigned char> File::getFileBytes()
{
    std::vector<unsigned char> fileBytes = std::vector<unsigned char>();
    
    if(valid)
    {
        while(true)
        {
            unsigned char byte;
            byte = fileStream->get();

            if(!fileStream->eof())
                fileBytes.push_back(byte);
            else
                break;
        }
    }

    return fileBytes;
}