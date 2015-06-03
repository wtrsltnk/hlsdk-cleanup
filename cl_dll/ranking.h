#ifndef RANKING_H
#define RANKING_H

typedef struct sRank
{
    int time;
    char playerName[32];

} tRank;

class Ranking
{
public:
    Ranking();
    virtual ~Ranking();

    void GetMapRanking(const char* mapid, const char* scoreserver);
    bool ParseRankingsJson(char* szJson);
    bool ParseOneRankingJson(char* szJson, int i);

    void SetKeyValue(int index, const char* key, const char* value);

    tRank* GetRankings() { return this->m_Rankings; }
private:
    char m_szScoreServer[256];
    char m_szMapId[32];
    tRank m_Rankings[5];

public:
    // We use this method for server side score sumbission
    static void SubmitScore(int time, const char* mapid, const char* playerid);
};

#endif // RANKING_H
