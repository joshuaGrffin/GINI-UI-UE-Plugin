// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GINIUIManagerSubsystem.generated.h"

class UGINIUIPolicy;
/**
 * 
 */
UCLASS(config = UIManagement, DefaultConfig)
class GINIUSERINTERFACE_API UGINIUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGINIUIManagerSubsystem() = default;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const UGINIUIPolicy* GetCurrentUIPolicy() const {return CurrentPolicy;};
	UGINIUIPolicy* GetCurrentUIPolicy(){return CurrentPolicy;};

	virtual void NotifyPlayerAdded(ULocalPlayer* NewPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* OldPlayer);
	virtual void NotifyPlayerDestroyed(ULocalPlayer* OldPlayer);

	virtual void OnCinematicBegin();
	virtual void OnCinematicEnd();
	
protected:
	void SwitchToPolicy(UGINIUIPolicy* InPolicy);
	
private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();
	
	UPROPERTY(Transient)
	TObjectPtr<UGINIUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY()
	TSoftClassPtr<UGINIUIPolicy> DefaultUIPolicy = nullptr;

	UPROPERTY()
	bool bShouldSyncWithPlayerHUDVisibility = false;
	
	FTSTicker::FDelegateHandle TickerHandle = nullptr;
};
