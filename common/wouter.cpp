#include <curl/curl.h>

char playerIdBuffer[64] = { 0 };

const char* GetEscapedPlayerId(const char* playerID)
{
    CURL* c;
    c = curl_easy_init();
    char* escaped = curl_easy_escape(c, playerID, 16);

    strcpy(playerIdBuffer, escaped);

    curl_free(escaped);
    curl_easy_cleanup( c );

    return playerIdBuffer;
}
