#ifndef GROUPLE_H
#define GROUPLE_H

#include "mangabase.h"
#include <curl/curl.h>
#include <string>
#include <vector>

class Grouple : public MangaBase
{
public:
    Grouple(CURL *c, const std::string& site, const std::string& url);

    void download_chapters(size_t begin_chapter, size_t end_chapter) override;

protected:
    std::string get_chapter_fullname(size_t i);

    void download_chapters_list() override;

    std::string get_first_chapter_url();

    size_t skip_chapters(size_t i, size_t begin_chapter, const std::string& chapter_mask);

    void download_chapter(size_t i) override;
};

#endif // GROUPLE_H
