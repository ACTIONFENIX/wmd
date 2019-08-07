#include "readmanga.h"
#include <algorithm>
#include <cassert>
#include <experimental/filesystem>
#include <iostream>

ReadManga::ReadManga(CURL *c, const std::string& url): SiteBase(c, "http://readmanga.me", url)
{

}

void ReadManga::download_chapters(size_t begin, size_t end)
{
    if (begin > end)
    {
        return;
    }
    download_main_page(m_url);

    std::string begin_url = std::string("<a href=\"/") + std::string(m_url.data() + m_site.size() + 1) + "/vol";
    auto i = m_main_page.find(begin_url) + begin_url.size();
    assert(i != begin_url.npos);
    while (m_main_page[i] != '"')
    {
        begin_url += m_main_page[i];
        ++i;
    }
    begin_url += '"';

    size_t chapter = 0;
    std::string chapter_mask = begin_url.substr(0, begin_url.find("vol") + 3);
    i = m_main_page.find(begin_url);
    i = m_main_page.find(begin_url, i + 1);
    while (chapter != begin)
    {
        i = m_main_page.rfind(chapter_mask);
        ++chapter;
    }
    while (chapter <= end && i != std::string::npos)
    {
        std::experimental::filesystem::create_directories(std::experimental::filesystem::path(m_main_page.substr(i + std::string(R"(<a href=")").size(), m_main_page.find('"', i + std::string(R"(<a href=")").size()) - i - std::string(R"(<a href=")").size()).c_str() + 1));
        download_chapter(m_site + m_main_page.substr(i + std::string(R"(<a href=")").size(), m_main_page.find('"', i + std::string(R"(<a href=")").size()) - i - std::string(R"(<a href=")").size()));
        i = m_main_page.rfind(chapter_mask, i - 1);
        ++chapter;
    }
    if (i == std::string::npos && chapter == begin)
    {
        std::cout << "No available chapters." << std::endl;
    }
}

void ReadManga::download_chapter(const std::string& chapter_url)
{
    download_chapter_page(chapter_url + "?mtr=1");
    if (m_chapter_page.find("Купите мангу") != chapter_url.npos)
    {
        //TODO: raise an exception
        std::cout << "You must buy this manga before you can download chapter " << chapter_url << std::endl;
        m_chapter_page.clear();
        return;
    }

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

        auto filename_end = image_url.find(".");
        auto filename_begin = image_url.rfind('/', filename_end) + 1;
        image_filename = image_url.substr(filename_begin,  filename_end - filename_begin + 4);
        image_filename = std::string(chapter_url.data() + m_site.size() + 1) + "/" + image_filename;
        download_image(image_server + image_url, image_filename);
        i = m_chapter_page.find("http:", i);
    }

    m_chapter_page.clear();
    std::cout << chapter_url.substr(chapter_url.rfind("/vol") + 1) << " ✓" << std::endl;
}
