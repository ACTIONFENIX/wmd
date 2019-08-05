#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <array>
#include "sitebase.h"
#include <curl/curl.h>

class Downloader
{
public:
    Downloader();

    Downloader(const Downloader&) = delete;

    Downloader& operator=(const Downloader&) = delete;

    ~Downloader();

    SiteBase *from_url(const std::string &url);

private:
    SiteBase *create_readmanga(CURL *c, const std::string& url) const;

private:
    using table_entry = std::pair<const char*, SiteBase *(Downloader::*)(CURL*, const std::string&) const>;

    static constexpr std::array<table_entry, 1> table =
    {
        {std::make_pair("readmanga", &Downloader::create_readmanga)}
    };

private:
    CURL *m_easy_curl;
};

#endif // DOWNLOADER_H
