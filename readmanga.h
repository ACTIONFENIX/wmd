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

    void download_chapter(size_type begin, size_type end) const;

    void download_volume(size_type begin, size_type end) const;
};

#endif // READMANGA_H
