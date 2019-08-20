#include "mangaexception.h"

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
    return m_err_text.c_str();
}

const char *PermissionDenied::what() const noexcept
{
    return m_err_text.c_str();
}
