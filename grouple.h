#ifndef READMANGA_H
#define READMANGA_H

#include "grouplebase.h"

class ReadManga : public Grouple
{
public:
    ReadManga(CURL *c, const std::string& url);
};

class MintManga : public Grouple
{
public:
    MintManga(CURL *c, const std::string& url);
};

class SelfManga : public Grouple
{
public:
    SelfManga(CURL *c, const std::string& url);
};

#endif // READMANGA_H
