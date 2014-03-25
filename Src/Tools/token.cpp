#include "../tools.h"

using namespace Tools;

vector<Token*> Token::loadedTokens;
string Token::EndToken = "#endToken";
string Token::BeginToken = "#firstToken";

Token::Token( Token* t) /// Stor constructor
{
    raw = t->gRaw();
    key = t->gKey();
}

Token::Token(const string& rToken) /// Throw away token
{
    raw = rToken;
    MakeToken();
}

Token::Token(const string& fileName , string fKey) /// Stored by key token
    : key(fKey)
{
    Token* t = Find(key);
    if(t == nullptr)
    {
        raw = File::LoadFile(fileName);
        loadedTokens.push_back( new Token(this) );
    }
    else
        raw = t->gRaw();

    MakeToken();
}

Token* Token::Find(string fKey)
{
    for(Token* t : loadedTokens)
        if(t->gKey() == fKey)
            return t;

    return nullptr;
}

void Token::MakeToken() /// Based on space | tab | new line
{
    string cVal = "";
    tokens.push_back("#firstToken");
    for (char ch : raw)
    {
        if(ch == ' ' || ch == '\n' || ch == '\t')
        {
            if(cVal != "")
            {
                tokens.push_back(cVal);
                cVal = "";
            }
        }
        else
            cVal += ch;
    }

    if(cVal != "")
        tokens.push_back(cVal);

    tokens.push_back("#endToken");
}

void Token::RemakeWithRules(vector<char> end,vector<char> split)
{
    Reset();
    string cVal = "";
    tokens.push_back("#firstToken");
    for (char ch : raw)
    {
        bool isEnded = false;

        for(char c : end)
            if(ch == c)
                isEnded = true;

        for(char c : split)
            if(ch == c)
            {
                if(cVal != "")
                {
                    tokens.push_back(cVal);
                    cVal = "";
                }
                cVal += ch;
            }

        if(isEnded)/// ch == ' ' || ch == '\n' || ch == '\t' || ch == '/'
        {
            if(cVal != "")
            {
                tokens.push_back(cVal);
                cVal = "";
            }
        }
        else
            cVal += ch;
    }

    if(cVal != "")
        tokens.push_back(cVal);

    tokens.push_back("#endToken");
}

string Token::Next() /// Return next token and update current token
{
    if(tokens.size() > indexer + 1)
        indexer++;
    else
        return EndToken;

    return tokens[indexer];
}

string Token::Peek(int i) /// Return indexer + i token
{
    unsigned int nIndexer = (indexer + i > 0 ? (indexer + i) : 0);
    if(tokens.size() >= nIndexer)
    {
        if(nIndexer >= 1)
            return tokens[nIndexer];
        else
            return BeginToken;
    }
    else
        return EndToken;
}

string Token::Current() /// Return current token
{
    return tokens[indexer];
}

int Token::GetNi()
{
    int i = 0;
    if(CanGNum())
        i = atoi(Next().c_str());

    return i;
}

vec2 Token::GetNVec2() /// Return vec2 based on next 2 tokens
{
    vec2 tVec(2,2);

    if( CanGVec2() )
    {
        tVec.x = atof(Next().c_str());
        tVec.y = atof(Next().c_str());
    }

    return tVec;
}

vec3 Token::GetNVec3() /// Return vec3 based on next 3 tokens
{
    vec3 tVec(0,0,0);

    if( CanGVec3() )
    {
        tVec.x = atof(Next().c_str());
        tVec.y = atof(Next().c_str());
        tVec.z = atof(Next().c_str());
    }

    return tVec;
}

bool Token::CanGNum() /// Is next token numeric
{
    return Str::isNum( Peek(1) );
}

bool Token::CanGVec2() /// Is next 2 token numeric
{
    return (CanGNum() && Str::isNum( Peek(2) ));
}

bool Token::CanGVec3() /// Is next 3 token numeric
{
    return (CanGVec2() && Str::isNum( Peek(3) ));
}

string Token::gRaw()
{
    return raw;
}

string Token::gKey()
{
    return key;
}

void Token::Reset() /// Reset for next use
{
    indexer = 0;
    tokens.clear();
}

void Token::Clear()
{
    indexer = 0;
    tokens.clear();
}

void Token::PrintTokens() /// Print all tokens
{
    for(string val : tokens)
        cout << val << endl;
}
