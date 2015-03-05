#include "../tools.h"

using namespace Tools;

string File::LoadFile(const string& fileName)
{
    string whole = "";
    fstream file(fileName.c_str());

    //Logger::Info("Load File -> " + fileName);

    if(file.is_open())
    {
        while(!file.eof())
        {
            int ch = file.get();
            if(ch != -1)
                whole += ch;
        }
        file.close();
    }
    else
        Logger::Error("Can't read file : " + fileName);

    return whole;
}
void File::SaveFile(const string& fileName,const string& data)
{
    fstream file(fileName.c_str(),std::fstream::trunc | std::fstream::out);
    if(file.is_open())
    {
        file << data;
        file.close();
    }
    else
        Logger::Error("Can't save file : " + fileName);
}
