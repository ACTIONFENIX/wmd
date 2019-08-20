#ifndef MANGAEXCEPTION_H
#define MANGAEXCEPTION_H

#include <exception>
#include <string>
#include <curl/curl.h>

class MangaException: public std::exception
{
public:
    const char *what() const noexcept override = 0;

    ~MangaException() override = default;
};

class CURLError: public MangaException
{
public:
    CURLError(CURLcode code): m_code(code) {}

    const char *what() const noexcept override;

    ~CURLError() override = default;

private:
    CURLcode m_code;
};

class ParseError: public MangaException
{
public:
    ParseError(const std::string& error): m_error(error) {}

    const char *what() const noexcept override;

    ~ParseError() override = default;

private:
    std::string m_error;
};

class NoChapters: public MangaException
{
public:
    const char *what() const noexcept override;

    ~NoChapters() override = default;
};

class NonFreeChapter: public MangaException
{
public:
    NonFreeChapter(const std::string& chapter_url): m_err_text(std::string("You must buy this manga before you can download chapter ") + chapter_url) {}

    const char *what() const noexcept override;

    ~NonFreeChapter() override = default;

private:
    std::string m_err_text;
};

class PermissionDenied: public MangaException
{
public:
    PermissionDenied(const std::string& file): m_err_text(std::string("Couldn't create ") + file) {}

    const char *what() const noexcept override;

    ~PermissionDenied() override = default;

private:
    std::string m_err_text;
};

#endif // MANGAEXCEPTION_H
