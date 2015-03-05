#include "../tools.h"

using namespace Tools;

bool Str::isNum(const string& str)
{
    bool val = true;

    for(char c : str)
        if(!isdigit(c))
            if(c != '.' && c != '-')
                val = false;

    return val;
}

void Str::AddHashStreamVec2( stringstream& stream, string key, vec2 val)
{
    stream << "\t"<< key << " " << val.x << " " << val.y << endl;
}
void Str::AddHashStreamVec3( stringstream& stream, string key, vec3 val)
{
    stream << "\t"<< key << " " << val.x << " " << val.y << " " << val.z << endl;
}

void Str::AddHashStreamVec4( stringstream& stream, string key, vec4 val)
{
    stream << "\t"<< key << " " << val.x << " " << val.y << " " << val.z << " " << val.w << endl;
}

string Str::ClearWhiteSpaces(const string& val)
{
    string nVal = "";

    for (int i : val)
        if(i != -1 && i != int(' '))
            nVal += i;

    return nVal;
}
