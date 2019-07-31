#ifndef ISITE_H
#define ISITE_H

#include <cstddef>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <fstream>

class SiteBase
{
public:
    using size_type = size_t;

public:
    SiteBase(CURL *c, const std::string& url);

    virtual ~SiteBase() = default;

    virtual void download_chapter(size_type begin, size_type end) const = 0;

    virtual void download_volume(size_type begin, size_type end) const = 0;

    virtual operator bool();

protected:
    static size_t write_data(void *from, size_t size, size_t nmemb, void *to);

    void download_image(const std::string& url, const std::string& location);

protected:
    CURL *m_easy_curl;
    const std::string m_url;
    std::vector<char> m_buffer;
    std::ofstream m_file;
    bool m_good = true;
};

#endif // ISITE_H
