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
    SiteBase(CURL *c, const std::string& site, const std::string& url);

    virtual ~SiteBase() = default;

    void set_location(const std::string& location);

    virtual void download_chapters(size_t begin, size_t end) = 0;

    virtual void download_chapter(const std::string& chapter_url) = 0;

    virtual operator bool();

protected:
    static size_t write_data_vector(void *from, size_t size, size_t nmemb, void *to);

    static size_t write_data_string(void *from, size_t size, size_t nmemb, void *to);

    void download_main_page(const std::string &url);

    void download_chapter_page(const std::string& url);

    void download_image(const std::string& url, const std::string& location);

protected:
    CURL *m_easy_curl;
    std::string m_site;
    const std::string m_url;
    std::string m_main_page;
    std::string m_chapter_page;
    std::vector<char> m_image;
    std::ofstream m_file;
    std::string m_location;
    bool m_good = true;
};

#endif // ISITE_H
