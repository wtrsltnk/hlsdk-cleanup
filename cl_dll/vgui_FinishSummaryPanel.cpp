#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

CFinishSummary :: CFinishSummary(int iTrans, int iRemoveMe, int x, int y, int wide, int tall)
    : CMenuPanel(iTrans, iRemoveMe, x, y, wide, tall), m_pActionSignal(0)
{
    // Get the scheme used for the Titles
    CSchemeManager *pSchemes = gViewPort->GetSchemeManager();

    // schemes
    SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Title Font" );

    m_pRetryButton = new CommandButton(
                gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ),
                wide * 0.1f,
                tall * 0.8f,
                wide * 0.2f,
                tall * 0.1f);
    m_pRetryButton->setParent( this );
    m_pRetryButton->addActionSignal( new CMenuHandler_StringCommand("restart") );

    m_pNextRunButton = new CommandButton(
                gHUD.m_TextMessage.BufferedLocaliseTextString( "Next run" ),
                wide * 0.71f,
                tall * 0.8f,
                wide * 0.2f,
                tall * 0.1f);
    m_pNextRunButton->setParent( this );
    this->m_pNextRunButton->setVisible(false);

    m_pFinalTime = new Label(
                gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ),
                wide * 0.1f,
                tall * 0.1f,
                wide * 0.81f,
                tall * 0.1f);
    m_pFinalTime->setParent( this );
    m_pFinalTime->setBgColor(Scheme::sc_primary1);
    m_pFinalTime->setFont(pSchemes->getFont( hTitleScheme ));

    for (int i = 0; i < 5; i++)
    {
        this->m_RankingLabels[i][0] = new Label(
                    gHUD.m_TextMessage.BufferedLocaliseTextString( "playername" ),
                    wide * 0.1f,
                    tall * 0.21f + (float(i) * tall * 0.11f),
                    wide * 0.6f,
                    tall * 0.1f);
        this->m_RankingLabels[i][0]->setParent(this);
        this->m_RankingLabels[i][0]->setBgColor(Scheme::sc_primary1);

        this->m_RankingLabels[i][1] = new Label(
                    gHUD.m_TextMessage.BufferedLocaliseTextString( "time" ),
                    wide * 0.71f,
                    tall * 0.21f + (float(i) * tall * 0.11f),
                    wide * 0.2f,
                    tall * 0.1f);
        this->m_RankingLabels[i][1]->setParent(this);
        this->m_RankingLabels[i][1]->setBgColor(Scheme::sc_primary1);
    }
}

void CFinishSummary :: SetFinalTime(int finaltime, const char* playerid)
{
    // Get the scheme used for the Titles
    CSchemeManager *pSchemes = gViewPort->GetSchemeManager();

    // schemes
    SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Title Font" );
    SchemeHandle_t hScoreboardText = pSchemes->getSchemeHandle( "Scoreboard Text" );

    int mil, totalsec, sec, min;
    char szFinalTime[64] = { 0 };

    // Calculate total milliseconds, total seconds and total minutes
    mil = finaltime % 1000;
    totalsec = (finaltime-mil) / 1000;
    sec = totalsec % 60;
    min = (totalsec-sec) / 60;

    // Make a nice string from all of these
    sprintf(szFinalTime, "%02d:%02d.%03d", min, sec, mil);

    this->m_pFinalTime->setText(szFinalTime);
    char tmpTime[5][32] = { 0 };

    tRank* r = gHUD.m_Clock.GetRanking().GetRankings();
    bool intop5 = false;
    for (int i = 0; i < 5; i++)
    {
        this->m_RankingLabels[i][0]->setText("");
        this->m_RankingLabels[i][0]->setFont(pSchemes->getFont( hScoreboardText ));
        this->m_RankingLabels[i][0]->setBgColor(Scheme::sc_secondary1);
        this->m_RankingLabels[i][1]->setText("");
        this->m_RankingLabels[i][1]->setFont(pSchemes->getFont( hScoreboardText ));
        this->m_RankingLabels[i][1]->setBgColor(Scheme::sc_secondary1);

        // Calculate total milliseconds, total seconds and total minutes
        mil = r->time % 1000;
        totalsec = (r->time-mil) / 1000;
        sec = totalsec % 60;
        min = (totalsec-sec) / 60;
        if ((r->time == 0 || r->time > finaltime) && intop5 == false)
        {
            this->m_RankingLabels[i][0]->setFont(pSchemes->getFont( hTitleScheme ));
            this->m_RankingLabels[i][0]->setBgColor(Scheme::sc_primary1);
            this->m_RankingLabels[i][0]->setText(gHUD.m_TextMessage.BufferedLocaliseTextString("Your new time!"));
            this->m_RankingLabels[i][1]->setFont(pSchemes->getFont( hTitleScheme ));
            this->m_RankingLabels[i][1]->setText(gHUD.m_TextMessage.BufferedLocaliseTextString(szFinalTime));
            this->m_RankingLabels[i][1]->setBgColor(Scheme::sc_primary1);
            intop5 = true;
        }
        else if (r->time != 0)
        {
            sprintf_s(tmpTime[i], "%02d:%02d.%03d", min, sec, mil);
            this->m_RankingLabels[i][0]->setText(gHUD.m_TextMessage.BufferedLocaliseTextString(r->playerName));
            this->m_RankingLabels[i][1]->setText(gHUD.m_TextMessage.BufferedLocaliseTextString(tmpTime[i]));
            r++;
        }
    }
}

void CFinishSummary :: SetNextRun(const char* nextRun)
{
    char cmd[MAX_COMMAND_SIZE] = { 0 };

    // Create the map change command
    strcpy_s(cmd, "map ");
    strcat_s(cmd, nextRun);

    // Create the action signal when its not available
    if (this->m_pActionSignal == 0)
    {
        this->m_pActionSignal = new CMenuHandler_StringCommand(cmd);
        m_pNextRunButton->addActionSignal( this->m_pActionSignal );
    }

    // Set the command
    this->m_pActionSignal->SetCommand(cmd);
    this->m_pNextRunButton->setVisible(true);
}

void CFinishSummary :: Open( void )
{
    CMenuPanel::Open();
}
