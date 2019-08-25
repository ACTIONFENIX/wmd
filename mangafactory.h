#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <array>
#include "mangabase.h"
#include <curl/curl.h>
#include <memory>

//returns concrete manga site downloader to work with
class MangaFactory: public IMangaBase
{
public:
    MangaFactory();

    MangaFactory(const MangaFactory&) = delete;

    MangaFactory& operator=(const MangaFactory&) = delete;

    ~MangaFactory();

    void set_location(const std::string& location);

    const std::vector<ChapterInfo>& get_chapters_info();

    void download_chapters(size_t begin, size_t end);

    void download_chapter(size_t i);

    void set_url(const std::string &url);

private:
    MangaBase *create_readmanga(CURL *c, const std::string& url) const;

    MangaBase *create_mintmanga(CURL *c, const std::string& url) const;

    MangaBase *create_selfmanga(CURL *c, const std::string& url) const;

private:
    using table_entry = std::pair<const char*, MangaBase *(MangaFactory::*)(CURL*, const std::string&) const>;

    static constexpr std::array<table_entry, 3> table =
    {
        std::make_pair("readmanga", &MangaFactory::create_readmanga),
        std::make_pair("mintmanga", &MangaFactory::create_mintmanga),
        std::make_pair("selfmanga", &MangaFactory::create_selfmanga)
    };

private:
    CURL *m_easy_curl;
    std::unique_ptr<MangaBase> m_manga_downloader;
    std::string m_location;
    std::string m_url;
};

#endif // DOWNLOADER_H
