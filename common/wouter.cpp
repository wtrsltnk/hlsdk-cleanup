#include <curl/curl.h>
#include <memory>

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

void SubmitPlayerScore(int time, const char* playerid, const char* mapid)
{
    char szPostFields[32] = { 0 };
    char szUrl[128] = { 0 };

    sprintf(szPostFields, "time=%d&playerid=%s&mapid=%s", time, playerid, mapid);
    sprintf(szUrl, "%s/submit-score/index.php", "localhost:55032");

    //*
    CURL* c;
    c = curl_easy_init();
    curl_easy_setopt( c, CURLOPT_URL, szUrl );
    curl_easy_setopt( c, CURLOPT_POSTFIELDS, szPostFields);
    curl_easy_perform( c );
    curl_easy_cleanup( c );
    //*/
}
