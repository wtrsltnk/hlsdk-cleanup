/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  hud_msg.cpp
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "r_efx.h"

#include "particleman.h"
extern IParticleMan *g_pParticleMan;

#include "vgui_TeamFortressViewport.h"
extern TeamFortressViewport *gViewPort;


#include <fstream>
#define MAX_CLIENTS 32

extern BEAM *pBeam;
extern BEAM *pBeam2;

/// USER-DEFINED SERVER MESSAGE HANDLERS

int CHud :: MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf )
{
	ASSERT( iSize == 0 );

	// clear all hud data
	HUDLIST *pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->Reset();
		pList = pList->pNext;
	}

	// reset sensitivity
	m_flMouseSensitivity = 0;

	// reset concussion effect
	m_iConcussionEffect = 0;

	return 1;
}

void CAM_ToFirstPerson(void);

void CHud :: MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf )
{
	CAM_ToFirstPerson();
}

void CHud :: MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf )
{
	// prepare all hud data
	HUDLIST *pList = m_pHudList;

	while (pList)
	{
		if ( pList->p )
			pList->p->InitHUDData();
		pList = pList->pNext;
	}

	if ( g_pParticleMan )
		 g_pParticleMan->ResetParticles();

	//Probably not a good place to put this.
	pBeam = pBeam2 = NULL;
}


int CHud :: MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_Teamplay = READ_BYTE();

	return 1;
}


int CHud :: MsgFunc_Damage(const char *pszName, int iSize, void *pbuf )
{
	int		armor, blood;
	Vector	from;
	int		i;
	float	count;
	
	BEGIN_READ( pbuf, iSize );
	armor = READ_BYTE();
	blood = READ_BYTE();

	for (i=0 ; i<3 ; i++)
		from[i] = READ_COORD();

	count = (blood * 0.5) + (armor * 0.5);

	if (count < 10)
		count = 10;

	// TODO: kick viewangles,  show damage visually

	return 1;
}

int CHud :: MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_iConcussionEffect = READ_BYTE();
	if (m_iConcussionEffect)
		this->m_StatusIcons.EnableIcon("dmg_concuss",255,160,0);
	else
		this->m_StatusIcons.DisableIcon("dmg_concuss");
	return 1;
}

int CHud :: MsgFunc_ClockStart( const char *pszName, int iSize, void *pbuf )
{
    int	action;

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

int CHud :: MsgFunc_ClockFinish( const char *pszName, int iSize, void *pbuf )
{
    int	action;
    int totalmil, mil, totalsec, sec, min;
    char szNextRun[64] = { 0 };
    char szFinalTime[64] = { 0 };
    char szPlayerID[16] = { 0 };
    char szSubmitScore[64] = { 0 };
    const char* szEncodedPlayerID = 0;

    // Stop demo recording
    gEngfuncs.pfnClientCmd("stop");

    // As soon as the clock is stopped we submit the score. We do this by sending this client's
    // Unique Id to the server in a "submit_score" message. The server will actually submit the
    // score.

    // Grab the playerID for sumbission
    gEngfuncs.GetPlayerUniqueID(1, szPlayerID);

    // Get the encoded version of the playerid
    szEncodedPlayerID = GetEscapedPlayerId(szPlayerID);

    // Send the playerID to the server so it can submit this score under this client's Id
    strcpy(szSubmitScore, "submit_score ");
    strcat(szSubmitScore, szEncodedPlayerID);
    gEngfuncs.pfnClientCmd(szSubmitScore);

    std::ofstream fs;
    fs.open("rjr\\playerid.txt", std::ios::out);
    fs << szEncodedPlayerID;
    fs.close();

    // Lets read the real message now
    BEGIN_READ( pbuf, iSize );
    action = READ_BYTE();
    totalmil = READ_LONG();

    // Calculate total milliseconds, total seconds and total minutes
    mil = totalmil % 1000;
    totalsec = (totalmil-mil) / 1000;
    sec = totalsec % 60;
    min = (totalsec-sec) / 60;

    // Make a nice string from all of these
    sprintf(szFinalTime, "%02d:%02d.%03d", min, sec, mil);

    // Sjhow the time in the VGUI menu
    gViewPort->m_pFinishSummaryPanel->SetFinalTime(szFinalTime);

    // Read the next map
    strcpy(szNextRun, READ_STRING());
    gViewPort->m_pFinishSummaryPanel->SetNextRun(szNextRun);

    // Reset the clock so it is not counting anymore
    this->m_bClockFinished = true;
    this->m_bClockStarted = false;
    this->m_flClockFinishTime = gHUD.m_flTime;

    return 1;
}
