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

void GetMapRanking(const char* mapID, const char* scoreserver)
{
    char szPostFields[256] = { 0 };
    sprintf(szPostFields, "mapid=%s", mapID);

    char url[128] = { 0 };
    sprintf(url, "%s/submit-score/top.php", scoreserver);

    printf("Getting map ranking: %s\n", url);

    //*
    CURL* c;
    c = curl_easy_init();
    curl_easy_setopt( c, CURLOPT_URL, url );
    curl_easy_setopt( c, CURLOPT_POSTFIELDS, szPostFields);
    curl_easy_perform( c );
    curl_easy_cleanup( c );
    //*/
}
