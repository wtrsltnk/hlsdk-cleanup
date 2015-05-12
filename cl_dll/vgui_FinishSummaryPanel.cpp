#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

CFinishSummary :: CFinishSummary(int iTrans, int iRemoveMe, int x, int y, int wide, int tall)
    : CMenuPanel(iTrans, iRemoveMe, x, y, wide, tall), m_pActionSignal(0)
{
    m_pRetryButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ), wide * 0.66f, tall * 0.75f, XRES(75), YRES(30));
    m_pRetryButton->setParent( this );
    m_pRetryButton->addActionSignal( new CMenuHandler_StringCommand("restart") );

    m_pNextRunButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Next run" ), wide * 0.33f - XRES(75), tall * 0.75f, XRES(75), YRES(30));
    m_pNextRunButton->setParent( this );

    m_pFinalTime = new Label( gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ), wide * 0.5f - XRES(37), tall * 0.45f, XRES(74), YRES(30));
    m_pFinalTime->setParent( this );

    m_pQuitButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Quit" ), wide - XRES(70), tall - XRES(40), XRES(55), YRES(30));
    m_pQuitButton->setParent( this );
    m_pQuitButton->addActionSignal( new CMenuHandler_StringCommand("disconnect") );
}

void CFinishSummary :: SetFinalTime(const char* finaltime)
{
    this->m_pFinalTime->setText(finaltime);
}

void CFinishSummary :: SetNextRun(const char* nextRun)
{
    char cmd[MAX_COMMAND_SIZE] = { 0 };

    // Create the map change command
    strcpy(cmd, "map ");
    strcat(cmd, nextRun);

    // Create the action signal when its not available
    if (this->m_pActionSignal == 0)
    {
        this->m_pActionSignal = new CMenuHandler_StringCommand(cmd);
        m_pNextRunButton->addActionSignal( this->m_pActionSignal );
    }

    // Set the command
    this->m_pActionSignal->SetCommand(cmd);
}
