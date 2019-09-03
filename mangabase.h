#ifndef MANGABASE_H
#define MANGABASE_H

#include <cstddef>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <fstream>
#include <atomic>

//fullname - "volumes_number - chapters_number chapters_title"
struct ChapterInfo
{
    std::string fullname;
    std::string url;
};

class IMangaBase
{
public:
    virtual ~IMangaBase() = default;

    //set directory where to download chapters
    virtual void set_location(const std::string& location) = 0;

    virtual const std::vector<ChapterInfo>& get_chapters_info() = 0;

    virtual void download_chapter(size_t i) = 0;

    virtual void set_compressed(bool compressed) = 0;

    virtual bool is_compressed() const = 0;

    virtual void stop() = 0;

    virtual bool is_stopped() const = 0;
};

class MangaBase: public IMangaBase
{
public:
    MangaBase(CURL *c, const std::string& site, const std::string& url);

    virtual ~MangaBase() = default;

    //set directory where to download chapters
    void set_location(const std::string& location) override;

    const std::vector<ChapterInfo>& get_chapters_info() override;

    void download_chapter(size_t i) override = 0;

    void set_compressed(bool compressed) override;

    bool is_compressed() const override;

    void stop() override;

    bool is_stopped() const override;

protected:
    virtual void download_chapters_list() = 0;

    //append downloaded block of bytes (webpage or image) to container to with type T
    template<typename T>
    static size_t write_data(void *from, size_t size, size_t nmemb, void *to);

    //check if curl operation returned CURL_OK
    void examine_curl_code(CURLcode code);

    void download_main_page(const std::string &url);

    void download_chapter_page(const std::string& url);

    void download_image(const std::string& url, const std::string& filename);

    void compress(const std::string& path);

protected:
    CURL *m_easy_curl;
    std::string m_site;
    const std::string m_url;
    std::string m_main_page;
    std::string m_chapter_page;
    std::vector<char> m_image;
    std::ofstream m_file;
    std::string m_location;
    std::vector<ChapterInfo> m_chapter_list;
    bool m_compressed = false;
    std::atomic<bool> m_stop = false;
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
