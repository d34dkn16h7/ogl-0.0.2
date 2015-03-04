#ifndef TOOLS_H
#define TOOLS_H

#include <glm/glm.hpp>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
using namespace glm;

namespace Tools
{
    struct ObjectInfo
    {
    public:
        string name;
        string prefabPath;
        ObjectInfo(string tPath, string tName) : name(tName) , prefabPath(tPath) {}
    };

    struct Rect
    {
    public:
        float xmi,xma;
        float ymi,yma;
        Rect() : xmi(-1), xma(1), ymi(-1), yma(1) {}
        Rect(float w,float h) : xmi(-w), xma(w), ymi(-h), yma(h) {}
        Rect(float xm1,float ym1,float xm2 , float ym2) : xmi(xm1), xma(xm2), ymi(ym1), yma(ym2) {}
        Rect(const Rect& r)
        {
            xmi = r.xmi;
            xma = r.xma;
            ymi = r.ymi;
            yma = r.yma;
        }
        Rect(const vec2 val) : Rect(val.x,val.y) {}
        Rect(const vec3 val) : Rect(val.x,val.y) {}
        Rect(const Rect& r, const vec3& s) : Rect(r) /// Scaled
        {
            Scale(s);
        }
        void Scale(const vec3& val)
        {
            xmi = xmi * val.x;
            xma = xma * val.x;
            ymi = ymi * val.y;
            yma = yma * val.y;
        }
        void AddOffset(const vec3& val)
        {
            xmi += val.x;
            xma += val.x;
            ymi += val.y;
            yma += val.y;
        }
    };

    class File
    {
    public:
        static string LoadFile( const string&);
        static void SaveFile( const string&, const string&);
    };

    class Str
    {
    public:
        static bool isNum( const string&);
        static void AddHashStreamVec3( stringstream&, string, vec3);
        static void AddHashStreamVec4( stringstream&, string, vec4);
        static string ClearWhiteSpaces( const string&);
    };

    class Token
    {
    private:
        static vector<Token*> loadedTokens;

        unsigned int indexer = 0;
        string raw; /// Raw source string
        string key; /// Retrieve key for token

        Token* Find(string);
        void MakeToken(); /// Based on space | tab | new line
    public:
        vector<string> tokens;

        static string EndToken;
        static string BeginToken;

        Token( Token*);
        Token( const string&);
        Token( const string&, string); /// Register this by tId

        bool operator==(const string& str )
            {return tokens[indexer] == str;}
        bool operator!=(const string& str )
            {return tokens[indexer] != str;}

        string Next(); /// Return next token and update current token
        string Peek( int); /// Return indexer + i token
        string Current(); /// Return current token

        bool CanGNum(); /// Is next token numeric
        bool CanGVec2(); /// Is next 2 token numeric
        bool CanGVec3(); /// Is next 3 token numeric

        int GetNi();
        vec2 GetNVec2(); /// Return vec2 based on next 2 tokens
        vec3 GetNVec3(); /// Return vec3 based on next 3 tokens

        string gRaw();
        string gKey();

        void RemakeWithRules( vector<char>, vector<char>);
        void Reset(); /// Reset for next use
        void Clear(); /// Clear all data
        void PrintTokens(); /// Print all tokens
    };

    class StoredToken : public Token /// Just for code readability
    {
    public:
        StoredToken( const string& fName, string key) : Token(fName,key) {}
    };

    class Logger
    {
    private:
        static void Log(const string str)
        {
            cout << str << endl;
        }
    public:
        static void Error(const string str)
        {
            Log("Error -> " + str);
        }
        static void Warning(const string str)
        {
            Log("Warning -> " + str);
        }
    };

    class Settings
    {
    public:
        static vector<ObjectInfo> objectInfos;

        static bool loadTextures;
        static bool TextureLoadModern;
        static string mapFile;
        static string vertexShaderFileName;
        static string fragmentShaderFileName;

        static void LoadSettings();
        static void LoadFiles();

        static bool isObject(string);
        static string gPrefabPath(string);
    };
}
#endif // TOOLS_H
