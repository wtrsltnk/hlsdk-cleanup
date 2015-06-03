#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"
#include "event_api.h"
#include <fstream>

#include "vgui_TeamFortressViewport.h"
extern TeamFortressViewport *gViewPort;

DECLARE_MESSAGE( m_Clock, ClockInit );
DECLARE_MESSAGE( m_Clock, ClockStart );
DECLARE_MESSAGE( m_Clock, ClockFinish );

cvar_t	*rjr_playerid;

int CHudClock :: Init()
{
    gHUD.AddHudElem(this);

    rjr_playerid = gEngfuncs.pfnRegisterVariable ( "rjr_playerid", "0", FCVAR_CLIENTDLL );

    HOOK_MESSAGE( ClockInit );
    HOOK_MESSAGE( ClockStart );
    HOOK_MESSAGE( ClockFinish );

    this->m_iFlags = HUD_ACTIVE;

    gEngfuncs.pfnClientCmd("exec rjr.cfg");

    return 1;
}

int CHudClock :: VidInit()
{
    // Reset the clock variables everytime a map starts
    this->m_bClockStarted = this->m_bClockFinished = false;
    this->m_flClockStartTime = this->m_flClockFinishTime = 0.0f;

    return 1;
}

void CHudClock :: Reset()
{ }

void CHudClock :: InitHUDData()
{
    gEngfuncs.pfnClientCmd("update_map_ranking");
}

char clock_time[32] = { 0 };

int CHudClock :: Draw(float flTime)
{
    int totalmil, mil, totalsec, sec, min;

    if (this->m_bClockStarted)
    {
        if (this->m_bClockFinished == false)
            totalmil = float((gHUD.m_flTime - this->m_flClockStartTime) * 1000.0f);
        else
            totalmil = float((this->m_flClockFinishTime - this->m_flClockStartTime) * 1000.0f);

        mil = totalmil % 1000;
        totalsec = (totalmil-mil) / 1000;
        sec = totalsec % 60;
        min = (totalsec-sec) / 60;
        sprintf(clock_time, "%02d:%02d.%03d", min, sec, mil);

        gHUD.DrawHudString(20, 20, 0, clock_time, 100, 100, 100 );
    }
    else
    {
        tRank* rank = this->m_Ranking.GetRankings();
        mil = rank->time % 1000;
        totalsec = (rank->time-mil) / 1000;
        sec = totalsec % 60;
        min = (totalsec-sec) / 60;
        sprintf(clock_time, "Time to beat: %02d:%02d.%03d", min, sec, mil);

        gHUD.DrawHudString(20, 20, 0, clock_time, 100, 100, 100 );
    }

    return 1;
}

char szMapId[32] = { 0 };
char szScoreServer[256] = { 0 };

int CHudClock :: MsgFunc_ClockInit( const char *pszName, int iSize, void *pbuf )
{
    BEGIN_READ( pbuf, iSize );
    strncpy(szMapId, READ_STRING(), 32);
    strncpy(szScoreServer, READ_STRING(), 256);

    this->m_Ranking.GetMapRanking(szMapId, szScoreServer);

    return 1;
}

int CHudClock :: MsgFunc_ClockStart( const char *pszName, int iSize, void *pbuf )
{
    int action;

    // Start demo recording
    gEngfuncs.pfnClientCmd("record demos/lastdemo");

    // Start reading the message. This has an action, but is not used right now.
    BEGIN_READ( pbuf, iSize );
    action = READ_BYTE();

    // Isn't the clock already started? If not so, start it and save the start time
    if (this->m_bClockStarted == false)
    {
        this->m_bClockStarted = true;
        this->m_bClockFinished = false;
        this->m_flClockStartTime = gHUD.m_flTime;
    }

    return 1;
}

extern const char* GetEscapedPlayerId(const char* playerID);

int CHudClock :: MsgFunc_ClockFinish( const char *pszName, int iSize, void *pbuf )
{
    int totalmil;
    int	action;
    char szNextRun[64] = { 0 };
    char szSubmitScore[64] = { 0 };

    // Stop demo recording
    gEngfuncs.pfnClientCmd("stop");

    // As soon as the clock is stopped we submit the score. We do this by sending this client's
    // Unique Id to the server in a "submit_score" message. The server will actually submit the
    // score.

    // Get the encoded version of the playerid
    this->m_szEncodedPlayerID = GetEscapedPlayerId(rjr_playerid->string);

    // Send the playerID to the server so it can submit this score under this client's Id
    strcpy(szSubmitScore, "submit_score ");
    strcat(szSubmitScore, this->m_szEncodedPlayerID);
    gEngfuncs.pfnClientCmd(szSubmitScore);

    // Lets read the real message now
    BEGIN_READ( pbuf, iSize );
    action = READ_BYTE();
    totalmil = READ_LONG();

    // Show the time in the VGUI menu
    gViewPort->m_pFinishSummaryPanel->SetFinalTime(totalmil, rjr_playerid->string);

    // Read the next map
    strcpy(szNextRun, READ_STRING());
    gViewPort->m_pFinishSummaryPanel->SetNextRun(szNextRun);

    // Reset the clock so it is not counting anymore
    this->m_bClockFinished = true;
    this->m_bClockStarted = false;
    this->m_flClockFinishTime = gHUD.m_flTime;

    return 1;
}
