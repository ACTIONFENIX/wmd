#include <iostream>
#include <curl/curl.h>
#include "sitebase.h"
#include "downloader.h"

/*using namespace std;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    vector<char> *pbuffer = static_cast<vector<char>*>(userp);

    cout << "size = " << size << endl;
    cout << "nmemb = " << nmemb << endl;
    pbuffer->reserve(pbuffer->size() + size * nmemb);
    pbuffer->insert(pbuffer->end(), static_cast<char*>(buffer), static_cast<char*>(buffer) + size * nmemb);
    return size * nmemb;
}

int main()
{
    std::vector<char> buffer;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();
    if (!handle)
    {
        cout << "error init" << endl;
        return 1;
    }

    curl_easy_setopt(handle, CURLOPT_URL, "http://h12.mangas.rocks/auto/03/01/59/rtower_of_god_ch00_1.jpg?t=1564613449&u=0&h=lAAqOIy5R5R1QJfP74NHVw");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
    CURLcode code = curl_easy_perform(handle);
    if (code != CURLE_OK)
    {
        cout << "error perform" << endl;
        return 1;
    }

    curl_easy_cleanup(handle);
    curl_global_cleanup();

    ofstream file("pic.jpg");
    cout << "size = " << buffer.size() << endl;
    file.write(buffer.data(), buffer.size());
    return 0;
}
*/

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        Downloader d;

        SiteBase *site = d.from_url(argv[1]);
        site->download_chapters(0, 1);
        //TODO
    }
    else
    {
        std::cout << "Usage: " << argv[1] << " <url>" << std::endl;
    }
    return 0;
}
