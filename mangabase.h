#ifndef MANGABASE_H
#define MANGABASE_H

#include <cstddef>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <fstream>

class MangaBase
{
public:
    MangaBase(CURL *c, const std::string& site, const std::string& url);

    virtual ~MangaBase() = default;

    void set_location(const std::string& location);

    virtual void download_chapters(size_t begin, size_t end) = 0;

    virtual void download_chapter(const std::string& chapter_url) = 0;

    virtual operator bool();

protected:
    template<typename T>
    static size_t write_data(void *from, size_t size, size_t nmemb, void *to);

    void examine_curl_code(CURLcode code);

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

template <typename T>
size_t MangaBase::write_data(void *from, size_t size, size_t nmemb, void *to)
{
    T *pbuffer = static_cast<T*>(to);

    pbuffer->reserve(pbuffer->size() + size * nmemb);
    pbuffer->insert(pbuffer->end(), static_cast<char*>(from), static_cast<char*>(from) + size * nmemb);
    return size * nmemb;
}

#endif // MANGABASE_H
