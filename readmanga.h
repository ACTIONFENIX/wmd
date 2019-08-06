#ifndef READMANGA_H
#define READMANGA_H

#include "sitebase.h"
#include <curl/curl.h>
#include <string>
#include <vector>

class ReadManga : public SiteBase
{
public:
    ReadManga(CURL *c, const std::string& url);

    void download_chapters(size_t begin, size_t end) override;

    void download_chapter(const std::string& chapter_url) override;
};

#endif // READMANGA_H
