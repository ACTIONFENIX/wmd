#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <array>
#include "mangabase.h"
#include <curl/curl.h>

class MangaFactory
{
public:
    MangaFactory();

    MangaFactory(const MangaFactory&) = delete;

    MangaFactory& operator=(const MangaFactory&) = delete;

    ~MangaFactory();

    MangaBase *from_url(const std::string &url);

private:
    MangaBase *create_readmanga(CURL *c, const std::string& url) const;

private:
    using table_entry = std::pair<const char*, MangaBase *(MangaFactory::*)(CURL*, const std::string&) const>;

    static constexpr std::array<table_entry, 1> table =
    {
        {std::make_pair("readmanga", &MangaFactory::create_readmanga)}
    };

private:
    CURL *m_easy_curl;
};

#endif // DOWNLOADER_H
