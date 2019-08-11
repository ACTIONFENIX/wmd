#include "mangafactory.h"
#include "readmanga.h"
#include "mangaexception.h"
#include <curl/curl.h>
#include <algorithm>

MangaFactory::MangaFactory()
{
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        throw CURLInitError();
    }
    m_easy_curl = curl_easy_init();
    if (m_easy_curl == nullptr)
    {
        throw CURLInitError();
    }
}

MangaFactory::~MangaFactory()
{
    curl_easy_cleanup(m_easy_curl);
    curl_global_cleanup();
}

MangaBase *MangaFactory::from_url(const std::string &url)
{
    auto it = find_if(table.begin(), table.end(), [&url](auto p)
    {
        return url.find(p.first) != url.npos;
    });
    if (it != table.end())
    {
        return ((*this).*(it->second))(m_easy_curl, url);
    }
    else
    {
        return nullptr;
    }
}

MangaBase *MangaFactory::create_readmanga(CURL *c, const std::string& url) const
{
    return new ReadManga(c, url);
}
