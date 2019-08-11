#include "grouplebase.h"
#include "mangaexception.h"
#include <algorithm>
#include <experimental/filesystem>
#include <iostream>

Grouple::Grouple(CURL *c, const std::string& site, const std::string& url): MangaBase(c, site, url)
{

}

std::string Grouple::get_first_chapter_url()
{
    std::string first_chapter_url = std::string("<a href=\"/") + std::string(m_url.data() + m_site.size() + 1) + "/vol";
    auto i = m_main_page.find(first_chapter_url) + first_chapter_url.size();
    if (i == std::string::npos)
    {
        throw ParseError("Couldn't find any chapters in the web page.");
    }
    while (m_main_page[i] != '"')
    {
        first_chapter_url += m_main_page[i];
        ++i;
    }
    first_chapter_url += '"';
    return first_chapter_url;
}

size_t Grouple::skip_chapters(size_t i, size_t begin_chapter, const std::string& chapter_mask)
{
    size_t chapter = 0;

    while (chapter != begin_chapter)
    {
        i = m_main_page.rfind(chapter_mask, i - 1);
        ++chapter;
    }
    return i;
}

void Grouple::download_chapters(size_t begin_chapter, size_t end_chapter)
{
    if (begin_chapter > end_chapter)
    {
        throw IncorrectChaptersInterval();
    }
    download_main_page(m_url);

    std::string first_chapter_url = get_first_chapter_url();
    std::string chapter_mask = first_chapter_url.substr(0, first_chapter_url.find("vol") + 3);
    auto i = m_main_page.find(first_chapter_url);
    auto begin_chapters_url_block = m_main_page.find("expandable chapters-link");
    i = m_main_page.find(first_chapter_url, i + 1);

    i = skip_chapters(i, begin_chapter, chapter_mask);
    size_t chapter = begin_chapter;
    while (chapter <= end_chapter && i != std::string::npos && i > begin_chapters_url_block)
    {
        std::experimental::filesystem::create_directories(std::experimental::filesystem::path(m_main_page.substr(i + std::string(R"(<a href=")").size(), m_main_page.find('"', i + std::string(R"(<a href=")").size()) - i - std::string(R"(<a href=")").size()).c_str() + 1));
        download_chapter(m_site + m_main_page.substr(i + std::string(R"(<a href=")").size(), m_main_page.find('"', i + std::string(R"(<a href=")").size()) - i - std::string(R"(<a href=")").size()));
        i = m_main_page.rfind(chapter_mask, i - 1);
        ++chapter;
    }
    if (i == std::string::npos && chapter == begin_chapter)
    {
        throw NoChapters();
    }
}

void Grouple::download_chapter(const std::string& chapter_url)
{
    std::cout << chapter_url.substr(chapter_url.rfind("/vol") + 1) << " " << std::flush;
    download_chapter_page(chapter_url + "?mtr=1");
    if (m_chapter_page.find("Купите мангу") != chapter_url.npos)
    {
        m_chapter_page.clear();
        throw NonFreeChapter(chapter_url);
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

        auto filename_end = image_url.find("?");
        auto filename_begin = image_url.rfind('/', filename_end) + 1;
        image_filename = image_url.substr(filename_begin,  filename_end - filename_begin);
        image_filename = std::string(chapter_url.data() + m_site.size() + 1) + "/" + image_filename;
        download_image(image_server + image_url, image_filename);
        i = m_chapter_page.find("http:", i);
    }

    m_chapter_page.clear();
    std::cout << "✓" << std::endl;
}
