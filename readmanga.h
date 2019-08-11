#ifndef READMANGA_H
#define READMANGA_H

#include "mangabase.h"
#include <curl/curl.h>
#include <string>
#include <vector>

class ReadManga : public MangaBase
{
public:
    ReadManga(CURL *c, const std::string& url);

    void download_chapters(size_t begin_chapter, size_t end_chapter) override;

private:
    std::string get_first_chapter_url();

    size_t skip_chapters(size_t i, size_t begin_chapter, const std::string& chapter_mask);

    void download_chapter(const std::string& chapter_url) override;
};

#endif // READMANGA_H
