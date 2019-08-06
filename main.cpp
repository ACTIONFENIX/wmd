#include <iostream>
#include <curl/curl.h>
#include "sitebase.h"
#include "downloader.h"
#include <experimental/filesystem>

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        Downloader d;

        SiteBase *site = d.from_url(argv[1]);
        if (site)
        {
            site->download_chapters(0, 5);
        }
        else
        {
            std::cout << "Site is not supported." << std::endl;
        }
        delete site;
    }
    else
    {
        std::cout << "Usage: " << std::experimental::filesystem::path(argv[0]).filename() << " <url>" << std::endl;
    }
    return 0;
}
