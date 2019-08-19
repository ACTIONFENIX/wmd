#include "grouple.h"

ReadManga::ReadManga(CURL *c, const std::string& url): Grouple(c, "http://readmanga.me", url)
{

}

MintManga::MintManga(CURL *c, const std::string& url): Grouple(c, "http://mintmanga.com", url)
{

}

SelfManga::SelfManga(CURL *c, const std::string& url): Grouple(c, "https://selfmanga.ru", url)
{

}
