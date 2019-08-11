#include "mangabase.h"
#include "mangaexception.h"

void MangaBase::examine_curl_code(CURLcode code)
{
    if (code != CURLE_OK)
    {
        throw CURLError(code);
    }
}

void MangaBase::download_main_page(const std::string &url)
{
    if (m_main_page.empty())
    {
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data<std::string>);
        curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_main_page);
        examine_curl_code(curl_easy_perform(m_easy_curl));
    }
}

void MangaBase::download_chapter_page(const std::string &url)
{
    if (m_chapter_page.empty())
    {
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data<std::string>);
        curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_chapter_page);
        examine_curl_code(curl_easy_perform(m_easy_curl));
    }
}

MangaBase::MangaBase(CURL *c, const std::string& site, const std::string &url): m_easy_curl(c), m_site(site), m_url(url)
{
    //enable redirects
    curl_easy_setopt(m_easy_curl, CURLOPT_FOLLOWLOCATION, true);
}

void MangaBase::set_location(const std::string &location)
{
    m_location = location;
}

MangaBase::operator bool()
{
    return m_good;
}

void MangaBase::download_image(const std::string &url, const std::string &location)
{
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data<std::vector<char>>);
    curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_image);
    examine_curl_code(curl_easy_perform(m_easy_curl));

    m_file.open(location);
    //TODO: add error management
    m_file.write(m_image.data(), m_image.size());
    m_file.close();
    m_image.clear();
}
