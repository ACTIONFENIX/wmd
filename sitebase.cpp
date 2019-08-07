#include "sitebase.h"
#include <cassert>

size_t SiteBase::write_data_vector(void *from, size_t size, size_t nmemb, void *to)
{
    std::vector<char> *pbuffer = static_cast<std::vector<char>*>(to);

    pbuffer->reserve(pbuffer->size() + size * nmemb);
    pbuffer->insert(pbuffer->end(), static_cast<char*>(from), static_cast<char*>(from) + size * nmemb);
    return size * nmemb;
}

size_t SiteBase::write_data_string(void *from, size_t size, size_t nmemb, void *to)
{
    std::string *pbuffer = static_cast<std::string*>(to);

    pbuffer->reserve(pbuffer->size() + size * nmemb);
    pbuffer->insert(pbuffer->end(), static_cast<char*>(from), static_cast<char*>(from) + size * nmemb);
    return size * nmemb;
}

void SiteBase::download_main_page(const std::string &url)
{
    if (m_main_page.empty())
    {
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data_string);
        curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_main_page);
        CURLcode code = curl_easy_perform(m_easy_curl);
        assert(code == CURLE_OK);
    }
}

void SiteBase::download_chapter_page(const std::string &url)
{
    if (m_chapter_page.empty())
    {
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data_string);
        curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_chapter_page);
        CURLcode code = curl_easy_perform(m_easy_curl);
        assert(code == CURLE_OK);
    }
}

SiteBase::SiteBase(CURL *c, const std::string& site, const std::string &url): m_easy_curl(c), m_site(site), m_url(url)
{
    //enable redirects
    curl_easy_setopt(m_easy_curl, CURLOPT_FOLLOWLOCATION, true);
}

void SiteBase::set_location(const std::string &location)
{
    m_location = location;
}

SiteBase::operator bool()
{
    return m_good;
}

void SiteBase::download_image(const std::string &url, const std::string &location)
{
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data_vector);
    curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_image);
    CURLcode code = curl_easy_perform(m_easy_curl);
    assert(code == CURLE_OK);
    m_file.open(location);
    m_file.write(m_image.data(), m_image.size());
    m_file.close();
    m_image.clear();
}
