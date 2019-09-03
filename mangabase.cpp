#include "mangabase.h"
#include "mangaexception.h"
#include "errorhandler.h"
#include <filesystem>
#include <ZipFile.h>

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
    if (m_location.back() != '/')
    {
        m_location.push_back('/');
    }
}

const std::vector<ChapterInfo> &MangaBase::get_chapters_info()
{
    m_stop = false;
    download_chapters_list();
    return m_chapter_list;
}

void MangaBase::download_image(const std::string &url, const std::string &filename)
{
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEFUNCTION, write_data<std::vector<char>>);
    curl_easy_setopt(m_easy_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_easy_curl, CURLOPT_WRITEDATA, &m_image);
    examine_curl_code(curl_easy_perform(m_easy_curl));

    m_file.open(m_location + filename);
    if (!m_file)
    {
        throw PermissionDenied(m_location + filename);
    }
    m_file.write(m_image.data(), m_image.size());
    m_file.close();
    m_image.clear();
}

void MangaBase::compress(const std::string &path)
{
    std::vector<std::filesystem::path> remove_files;
    std::string archive_name = std::filesystem::path(path) / (std::filesystem::path(path).filename().string() + ".zip");
    {
        //ensure archive file is present in the directory in order not to create it in directory loop
        std::ofstream file(archive_name);
        file.close();
    }
    std::filesystem::directory_iterator begin(path);
    std::filesystem::directory_iterator end;
    for (auto it = begin; it != end && !m_stop; ++it)
    {
        //do not compress itself
        if (it->path().extension() == ".zip")
        {
            continue;
        }

        ZipArchive::Ptr archive = ZipFile::Open(archive_name);
        ZipArchiveEntry::Ptr entry = archive->CreateEntry(it->path().filename());
        if (entry == nullptr)
        {
            ErrorHandler err(std::string("Can not add ") + it->path().string() + " to " + archive_name + ". Maybe there is already file with such name?");
            continue;
        }

        std::ifstream contentStream(it->path(), std::ios::binary);
        entry->SetCompressionStream(contentStream);
        ZipFile::SaveAndClose(archive, archive_name);
        remove_files.push_back(it->path());
    }
    if (m_stop)
    {
        return;
    }
    //removes everything from directory except archive
    for (auto i: remove_files)
    {
        std::filesystem::remove(i);
    }
    //move archive to the parent directory and delete previous
    std::filesystem::rename(archive_name, std::filesystem::path(archive_name).parent_path().parent_path() / std::filesystem::path(archive_name).filename());
    std::filesystem::remove(std::filesystem::path(archive_name).parent_path());
}

void MangaBase::set_compressed(bool compressed)
{
    m_compressed = compressed;
}

bool MangaBase::is_compressed() const
{
    return m_compressed;
}

void MangaBase::stop()
{
    m_stop = true;
}

bool MangaBase::is_stopped() const
{
    return m_stop;
}
