#include <iostream>
#include <curl/curl.h>
#include "mangabase.h"
#include "mangafactory.h"
#include <experimental/filesystem>

int main(int argc, char *argv[])
{
    if (argc >= 2 && argc <= 4)
    {
        int begin = 0;
        int end = -1;
        if (argc >= 3)
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
        if (argc >= 4)
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

        MangaFactory d;
        MangaBase *site = d.from_url(argv[1]);
        site->set_location("/home/michael/Рабочий стол");
        if (site)
        {
            try
            {
                site->download_chapters(static_cast<size_t>(begin), static_cast<size_t>(end));
            }
            catch (std::exception& e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
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
