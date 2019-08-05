#include "readmanga.h"
#include <algorithm>
#include <cassert>

ReadManga::ReadManga(CURL *c, const std::string& url): SiteBase(c, "http://readmanga.me", url)
{

}

void ReadManga::download_chapters(size_type begin, size_type end)
{
    download_main_page(m_url);

    std::string vol1_url = std::string("<a href=\"/") + std::string(m_url.data() + m_site.size() + 1) + "/vol";
    auto i = m_main_page.find(vol1_url) + vol1_url.size();
    assert(i != vol1_url.npos);
    while (m_main_page[i] != '"')
    {
        vol1_url += m_main_page[i];
        ++i;
    }

    size_type chapter = 0;
    std::string chapter_mask = vol1_url.substr(0, vol1_url.find("vol") + 3);
    i = m_main_page.rfind(vol1_url);
    i = m_main_page.rfind(vol1_url, i - 1);
    while (chapter != begin)
    {
        i = m_main_page.rfind(chapter_mask);
        ++chapter;
    }
    while (chapter != end + 1)
    {
        download_chapter(m_site + m_main_page.substr(i + std::string(R"(<a href=")").size(), m_main_page.find('"', i + std::string(R"(<a href=")").size()) - i - std::string(R"(<a href=")").size()));
        i = m_main_page.rfind(chapter_mask, i - 1);
        ++chapter;
    }
}

#include <iostream>

void ReadManga::download_chapter(const std::string& chapter_url)
{
    download_chapter_page(chapter_url);

    const std::string images_begin_section = "rm_h.init";
    const std::string images_end_section = "</script>";
    std::string image_server;
    std::string image_url;
    std::string image_filename;
    auto i = m_chapter_page.find(images_begin_section);
    auto end = m_chapter_page.find(images_end_section, i);

    i = m_chapter_page.find("http:", i);
    while (i < end)
    {
        image_server = m_chapter_page.substr(i, m_chapter_page.find('\'', i) - i);
        i = m_chapter_page.find('"', i) + 1;
        image_url = m_chapter_page.substr(i, m_chapter_page.find('"', i) - i);
        image_filename = image_url.substr(image_url.rfind('/', image_url.find(".jpg")) + 1, image_url.find(".jpg") + std::string(".jpg").size() - image_url.rfind('/', image_url.find(".jpg")) - 1);
        //image_filename = std::string(chapter_url.data() + m_site.size() + 1) + "/" + image_filename; doesn't create directories
        std::cout << "image_server = " << image_server << std::endl;
        std::cout << "image_url = " << image_url << std::endl;
        std::cout << "image_filename = " << image_filename << std::endl;
        download_image(image_server + image_url, image_filename);
        i = m_chapter_page.find("http:", i);
    }

    m_chapter_page.clear();
}
