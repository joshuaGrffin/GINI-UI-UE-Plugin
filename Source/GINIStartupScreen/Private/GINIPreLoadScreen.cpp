// The Griffin Initiative


#include "GINIPreLoadScreen.h"

#include "Misc/App.h"
#include "GINIPreLoadingScreenWidget.h"


void FGINIPreLoadScreen::Init()
{
	if (!GIsEditor && FApp::CanEverRender())
	{
		PreLoadScreen = SNew(SGINIPreLoadingScreenWidget);
	}
}
