#include "mangafactory.h"
#include "grouple.h"
#include "mangaexception.h"
#include "errorhandler.h"
#include <curl/curl.h>
#include <algorithm>

MangaFactory::MangaFactory()
{
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        ErrorHandler err("Couldn't initialize libcurl.", ErrorType::Critical);
    }
    m_easy_curl = curl_easy_init();
    if (m_easy_curl == nullptr)
    {
        ErrorHandler err("Couldn't initialize libcurl.", ErrorType::Critical);
    }
}

MangaFactory::~MangaFactory()
{
    curl_easy_cleanup(m_easy_curl);
    curl_global_cleanup();
}

void MangaFactory::set_location(const std::string& location)
{
    m_location = location;
    if (m_manga_downloader)
    {
        m_manga_downloader->set_location(m_location);
    }
}

const std::vector<ChapterInfo>& MangaFactory::get_chapters_info()
{
    if (m_manga_downloader)
    {
        return m_manga_downloader->get_chapters_info();
    }
    else
    {
        throw SiteNotSupported();
    }
}

void MangaFactory::download_chapters(size_t begin, size_t end)
{
    if (m_manga_downloader)
    {
        m_manga_downloader->download_chapters(begin, end);
    }
}

void MangaFactory::download_chapter(size_t i)
{
    if (m_manga_downloader)
    {
        m_manga_downloader->download_chapter(i);
    }
}

void MangaFactory::set_url(const std::string &url)
{
    auto it = find_if(table.begin(), table.end(), [&url](auto p)
    {
        return url.find(p.first) != url.npos;
    });
    if (it != table.end())
    {
        m_manga_downloader = std::unique_ptr<MangaBase>(((*this).*(it->second))(m_easy_curl, url));
        m_manga_downloader->set_location(m_location);
        m_url = url;
    }
    else
    {
        throw SiteNotSupported();
    }
}

MangaBase *MangaFactory::create_readmanga(CURL *c, const std::string& url) const
{
    return new ReadManga(c, url);
}

MangaBase *MangaFactory::create_mintmanga(CURL *c, const std::string& url) const
{
    return new MintManga(c, url);
}

MangaBase *MangaFactory::create_selfmanga(CURL *c, const std::string& url) const
{
    return new SelfManga(c, url);
}
