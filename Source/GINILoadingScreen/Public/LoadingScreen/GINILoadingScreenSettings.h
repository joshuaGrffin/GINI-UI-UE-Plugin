// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GINILoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "GINI Loading Screen Settings"))
class GINILOADINGSCREEN_API UGINILoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UGINILoadingScreenSettings();

	UPROPERTY(config, EditAnywhere, category = Display, meta = (MetaClass="/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenClassPath;
	
	UPROPERTY(config, EditAnywhere, Category = Display)
	int32 LoadingScreenZOrder = 10000;
	UPROPERTY(config, EditAnywhere, Category = Configuration, meta = (ForceUnits=s, ConsoleVariable="GINILoadingScreen.HoldLoadingScreenAdditionalSeconds"))
	float HoldLoadingScreenAdditonalSeconds = 2.0f;

	UPROPERTY(config, EditAnywhere, Category = Configuration, meta=(ForceUnits=s))
	float LoadingScreenHeartbeatHangDuration = 0.0f;

	UPROPERTY(config, EditAnywhere, Category = Configuration, meta=(ForceUnits=s))
	float LogLoadingScreenHeartbeatInterval = 5.0f;

	UPROPERTY(Transient, EditAnywhere, Category = Debugging, meta=(ConsoleVariable="GINILoadingScreen.LogLoadingScreenReasoneEveryFrame"))
	bool bLogLoadingScreenReasonEveryFrame = false;

	UPROPERTY(Transient, EditAnywhere, Category = Debugging, meta=(ConsoleVariable="GINILoadingScreen.ForceLoadingScreenVisible"))
	bool bForceLoadingScreenVisible = false;

	UPROPERTY(Transient, EditAnywhere, Category = Debugging)
	bool bHoldLoadingScreenAdditionalSecondsInEditor = false;
	UPROPERTY(Config, EditAnywhere, Category = Configuration)
	bool bForceTickLoadingScreenEvenInEditor = false;
	
};
