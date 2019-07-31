#include "readmanga.h"

ReadManga::ReadManga(CURL *c, const std::string& url): SiteBase(c, url)
{

}

void ReadManga::download_chapter(size_type begin, size_type end) const
{
    //curl_easy_setopt(m_easy_curl, CURLOPT_URL, "http://h12.mangas.rocks/auto/03/01/59/rtower_of_god_ch00_1.jpg?t=1564613449&u=0&h=lAAqOIy5R5R1QJfP74NHVw");
}

void ReadManga::download_volume(size_type begin, size_type end) const
{

}
