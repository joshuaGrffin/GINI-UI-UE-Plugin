
#include "GINIPreLoadScreen.h"
#include "PreLoadScreenBase.h"
#include "Misc/App.h"
#include "Modules/ModuleManager.h"
#include "PreLoadScreenManager.h"

#define LOCTEXT_NAMESPACE "FGINIStartupScreenModule"

class FGINIStartupScreenModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	bool IsGameModule() const override;

private:
	void OnPreLoadScreenManagerCleanUp();
	//#TODO: Replace with a real load screen class: 
	TSharedPtr<FGINIPreLoadScreen> PreLoadScreen;
};

void FGINIStartupScreenModule::StartupModule()
{
	if (!IsRunningDedicatedServer())
	{
		PreLoadScreen = MakeShared<FGINIPreLoadScreen>();
		PreLoadScreen->Init();
		FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get();
		if (!GIsEditor && FApp::CanEverRender() && PreLoadScreenManager)
		{
			PreLoadScreenManager->RegisterPreLoadScreen(PreLoadScreen);
			PreLoadScreenManager->OnPreLoadScreenManagerCleanUp.AddRaw(this, &FGINIStartupScreenModule::OnPreLoadScreenManagerCleanUp);
		}
	}
	
}

void FGINIStartupScreenModule::ShutdownModule()
{

}

bool FGINIStartupScreenModule::IsGameModule() const
{
	return true;
}

void FGINIStartupScreenModule::OnPreLoadScreenManagerCleanUp()
{
	PreLoadScreen.Reset();
	ShutdownModule();
}


#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGINIStartupScreenModule, GINIStartupScreen)