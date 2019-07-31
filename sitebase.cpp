#include "sitebase.h"
#include <cassert>

size_t SiteBase::write_data(void *from, size_t size, size_t nmemb, void *to)
{
    std::vector<char> *pbuffer = static_cast<std::vector<char>*>(to);

    pbuffer->reserve(pbuffer->size() + size * nmemb);
    pbuffer->insert(pbuffer->end(), static_cast<char*>(to), static_cast<char*>(to) + size * nmemb);
    return size * nmemb;
}

SiteBase::SiteBase(CURL *c, const std::string &url): m_easy_curl(c), m_url(url)
{
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_buffer);
}

SiteBase::operator bool()
{
    return m_good;
}

void SiteBase::download_image(const std::string &url, const std::string &location)
{
    curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
    CURLcode code = curl_easy_perform(m_easy_curl);
    assert(code != CURLE_OK);
    m_file.open(location);
    m_file.write(m_buffer.data(), m_buffer.size());
    m_file.close();
    m_buffer.clear();
}
