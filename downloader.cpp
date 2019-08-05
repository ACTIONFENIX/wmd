#include "downloader.h"
#include "readmanga.h"
#include <curl/curl.h>
#include <algorithm>

Downloader::Downloader()
{
    curl_global_init(CURL_GLOBAL_ALL);
    m_easy_curl = curl_easy_init();
}

Downloader::~Downloader()
{
    curl_easy_cleanup(m_easy_curl);
    curl_global_cleanup();
}

SiteBase *Downloader::from_url(const std::string &url)
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

SiteBase *Downloader::create_readmanga(CURL *c, const std::string& url) const
{
    return new ReadManga(c, url);
}
