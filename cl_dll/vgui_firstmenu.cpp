#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

CFirstMenu :: CFirstMenu(int iTrans, int iRemoveMe, int x, int y, int wide, int tall)
    : CMenuPanel(iTrans, iRemoveMe, x, y, wide, tall), m_pActionSignal(0)
{
    m_pRetryButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ), wide * 0.66f, tall * 0.75f, XRES(75), YRES(30));
    m_pRetryButton->setParent( this );
    m_pRetryButton->addActionSignal( new CMenuHandler_StringCommand("restart") );

    m_pNextRunButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Next run" ), wide * 0.33f, tall * 0.75f, XRES(75), YRES(30));
    m_pNextRunButton->setParent( this );

    m_pFinalTime = new Label( gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ), wide * 0.45f, tall * 0.45f, XRES(75), YRES(30));
    m_pFinalTime->setParent( this );
}

void CFirstMenu :: SetFinalTime(const char* finaltime)
{
    this->m_pFinalTime->setText(finaltime);
}

void CFirstMenu :: SetNextRun(const char* nextRun)
{
    char cmd[MAX_COMMAND_SIZE] = { 0 };
    strcpy(cmd, "map ");
    strcat(cmd, nextRun);
    if (this->m_pActionSignal == 0)
    {
        this->m_pActionSignal = new CMenuHandler_StringCommand(cmd);
        m_pNextRunButton->addActionSignal( this->m_pActionSignal );
    }
    else
    {
        this->m_pActionSignal->SetCommand(cmd);
    }
}
