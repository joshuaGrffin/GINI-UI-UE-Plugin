// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"

#include "GINIUISettings.Generated.h"

//#TODO: Think about how this will work with different screens and options like that. Should we use styles? 
USTRUCT(BlueprintType)
struct FGINIUITextSettingsData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 FontSize = 12;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float FontScale = 1.0f;
	
};

UCLASS(config = UIManagement)
class GINIUSERINTERFACE_API UUIManagerSettings : public UObject
{
	GENERATED_BODY()
public:
	UUIManagerSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<class UGINIUIPolicy> DefaultUIPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	bool bShouldSyncWithPlayerHUDVisibility = false;
	
	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<class UGININotifyModal> GenericNotificationModal = nullptr;
	
	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<class UGINIConfirmDenyModal> GenericConfirmDenyModal = nullptr;
};