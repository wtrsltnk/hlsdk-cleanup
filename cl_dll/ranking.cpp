#include "ranking.h"
#include "extdll.h"
#include "util.h"
#include <curl/curl.h>
#include <memory>

Ranking::Ranking()
{ }

Ranking::~Ranking()
{ }

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void Ranking::GetMapRanking(const char* mapid, const char* scoreserver)
{
    strncpy_s(this->m_szMapId, mapid, 32);
    strncpy_s(this->m_szScoreServer, scoreserver, 256);

	struct MemoryStruct chunk;

    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

	char szPostFields[256] = { 0 };
    sprintf_s(szPostFields, "mapid=%s", this->m_szMapId);

	char szUrl[128] = { 0 };
    sprintf_s(szUrl, "%s/challange/top", this->m_szScoreServer);

	CURL* c;
	c = curl_easy_init();
	curl_easy_setopt( c, CURLOPT_URL, szUrl );
	curl_easy_setopt( c, CURLOPT_POSTFIELDS, szPostFields);
	curl_easy_setopt( c, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt( c, CURLOPT_WRITEDATA, (void *)&chunk);

	curl_easy_perform( c );
	curl_easy_cleanup( c );

    ParseRankingsJson(chunk.memory);

	free(chunk.memory);
}

bool Ranking::ParseRankingsJson(char* szJson)
{
    char* start = szJson;
    char* end = szJson;
    int i = 0;

    while (start[0] != '\0')
    {
        while (start[0] != '\0' && start[0] != '{') start++;
        if (start[0] == '\0') return false;
        start++; // skip {

        end = start;
        while (end[0] != '\0' && end[0] != '}') end++;
        if (end[0] == '\0') return false;
        end[0] = '\0';

        if (ParseOneRankingJson(start, i++) == false)
            return false;
        start = end + 1;
    }

    return true;
}

bool Ranking::ParseOneRankingJson(char* szJson, int i)
{
    char key[32];
    char value[32];
    int j = 0;

    while (szJson[0] != '\0' && szJson[0] != '\"') szJson++;
    if (szJson[0] == '\0') return false;

    while (szJson[0] == '\"')
    {
        szJson++;   // skip "

        j = 0;
        while (szJson[0] != '\0' && szJson[0] != '\"')
        {
            key[j++] = szJson[0];
            szJson++;
        }
        if (szJson[0] == '\0') return false;
        szJson++;   // skip "
        key[j] = 0;

        while (szJson[0] != '\0' && szJson[0] != '\"') szJson++;
        if (szJson[0] == '\0') return false;

        szJson++;   // skip "

        j = 0;
        while (szJson[0] != '\0' && szJson[0] != '\"')
        {
            value[j++] = szJson[0];
            szJson++;
        }
        if (szJson[0] == '\0') return false;
        szJson++;   // skip "
        value[j] = 0;

        this->SetKeyValue(i, key, value);

        while (szJson[0] != '\0' && szJson[0] != '\"') szJson++;
    }

    return true;
}

void Ranking::SetKeyValue(int index, const char* key, const char* value)
{
    if (strcmp(key, "playerName") == 0)
        strncpy_s(this->m_Rankings[index].playerName, value, 32);

    if (strcmp(key, "time") == 0)
        this->m_Rankings[index].time = atoi(value);
}

char szPostFields[256] = { 0 };
char szUrl[256] = { 0 };

void Ranking::SubmitScore(int time, const char* mapid, const char* playerid)
{
    sprintf_s(szPostFields, "time=%d&playerid=%s&mapid=%s", time, playerid, mapid);
    sprintf_s(szUrl, "%s/challange/submit", CVAR_GET_STRING("sv_scoreserver"));

    //*
    CURL* c;
    c = curl_easy_init();
    curl_easy_setopt( c, CURLOPT_URL, szUrl );
    curl_easy_setopt( c, CURLOPT_POSTFIELDS, szPostFields);
    curl_easy_perform( c );
    curl_easy_cleanup( c );
    //*/
}

char playerIdBuffer[64] = { 0 };

const char* GetEscapedPlayerId(const char* playerID)
{
    CURL* c;
    c = curl_easy_init();
    char* escaped = curl_easy_escape(c, playerID, 16);

    strcpy_s(playerIdBuffer, escaped);

    curl_free(escaped);
    curl_easy_cleanup( c );

    return playerIdBuffer;
}
