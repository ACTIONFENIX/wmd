#ifndef GROUPLE_H
#define GROUPLE_H

#include "mangabase.h"
#include <curl/curl.h>
#include <string>
#include <vector>

//Base class for readmanga.me, mintmanga.com and selfmanga.ru downloaders
class Grouple : public MangaBase
{
public:
    Grouple(CURL *c, const std::string& site, const std::string& url);

protected:
    //function to fill ChapterInfo.fullname
    std::string get_chapter_fullname(size_t i);

    void download_chapters_list() override;

    //looks for text <a href="/manga_title/vol and gets from it first chapter url
    std::string get_first_chapter_url();

    void download_chapter(size_t i) override;
};

#endif // GROUPLE_H
