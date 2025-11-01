
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Settings/GINIUISettings.h"

#define LOCTEXT_NAMESPACE "FGINIUserInterfaceModule"


class FGINIUserInterfaceModule : public IModuleInterface
{
public:
	FGINIUserInterfaceModule();
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

FGINIUserInterfaceModule::FGINIUserInterfaceModule()
{
}

void FGINIUserInterfaceModule::StartupModule()
{

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "UI Management",
			LOCTEXT("RuntimeSettingsName", "UI Management"), LOCTEXT("RuntimeSettingsDescription", "Configure UI Management settings"),
			GetMutableDefault<UUIManagerSettings>());
	}
}

void FGINIUserInterfaceModule::ShutdownModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "UI Management");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGINIUserInterfaceModule, GINIUserInterface)