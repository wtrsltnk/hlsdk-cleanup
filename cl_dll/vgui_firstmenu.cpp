//#include "extdll.h"
//#include "util.h"
//#include "cbase.h"

#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

CFirstMenu :: CFirstMenu(int iTrans, int iRemoveMe, int x, int y, int wide, int tall)
    : CMenuPanel(iTrans, iRemoveMe, x, y, wide, tall)
{
    m_pRetryButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Retry" ), wide * 0.66f, tall * 0.75f, XRES(75), YRES(30));
    m_pRetryButton->setParent( this );
    m_pRetryButton->addActionSignal( new CMenuHandler_StringCommand("restart") );

    m_pNextRunButton = new CommandButton( gHUD.m_TextMessage.BufferedLocaliseTextString( "Next run" ), wide * 0.33f, tall * 0.75f, XRES(75), YRES(30));
    m_pNextRunButton->setParent( this );
    m_pNextRunButton->addActionSignal( new CMenuHandler_UseEntity("next_run") );
}
