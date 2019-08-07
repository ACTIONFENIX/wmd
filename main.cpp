#include <iostream>
#include <curl/curl.h>
#include "sitebase.h"
#include "downloader.h"
#include <experimental/filesystem>

int main(int argc, char *argv[])
{
    if (argc >= 2 && argc <= 4)
    {
        int begin = 0;
        int end = -1;
        if (argc == 3)
        {
            try
            {
                begin = std::stoi(argv[2]);
            }
            catch (std::exception)
            {
                std::cout << "first_chapter must be valid integer number" << std::endl;
                return 0;
            }
            if (begin < 0)
            {
                std::cout << "first_chapter must be 0 or positive number" << std::endl;
                return 0;
            }
        }
        if (argc == 4)
        {
            try
            {
                end = std::stoi(argv[3]);
            }
            catch (std::exception)
            {
                std::cout << "last_chapter must be valid integer number" << std::endl;
                return 0;
            }
            if (begin < 0)
            {
                std::cout << "last_chapter must be 0 or positive number" << std::endl;
                return 0;
            }
        }

        Downloader d;
        SiteBase *site = d.from_url(argv[1]);
        if (site)
        {
            site->download_chapters(static_cast<size_t>(begin), static_cast<size_t>(end));
        }
        else
        {
            std::cout << "Site is not supported." << std::endl;
        }
        delete site;
    }
    else
    {
        std::cout << "Usage: " << std::experimental::filesystem::path(argv[0]).filename() << " manga_url [first_chapter] [last_chapter]" << std::endl;
    }
    return 0;
}
