#include "mangaexception.h"

const char *CURLInitError::what() const noexcept
{
    return "Couldn't initialize libcurl.";
}

const char *CURLError::what() const noexcept
{
    return curl_easy_strerror(m_code);
}

const char *ParseError::what() const noexcept
{
    return m_error.c_str();
}

const char *NoChapters::what() const noexcept
{
    return "No available chapters.";
}

const char *NonFreeChapter::what() const noexcept
{
    return (std::string("You must buy this manga before you can download chapter ") + m_chapter_url).c_str();
}

const char *IncorrectChaptersInterval::what() const noexcept
{
    return "Incorrect chapters interval specified.";
}

const char *PermissionDenied::what() const noexcept
{
    return (std::string("Couldn't create ") + m_file).c_str();
}
