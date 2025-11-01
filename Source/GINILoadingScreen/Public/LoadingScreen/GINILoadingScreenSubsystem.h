// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GINILoadingScreenSubsystem.generated.h"

class IGINILoadingProcessInterface;
/**
 * 
 */
UCLASS()
class GINILOADINGSCREEN_API UGINILoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Subsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// End Subsystem Interface

	// FTickable Object Interface
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;
	// End FTickable Object Interface


	UFUNCTION(BlueprintCallable, Category = "LoadingScreen")
	FString GetDebugReasonForLoadingScreen() const
	{
		return DebugReasonForLoadingScreen;
	}

	UFUNCTION(BlueprintPure, Category = "LoadingScreen")
	bool GetIsLoadingScreenDisplayed() const
	{
		return bIsLoadingScreenDisplayed;
	}


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityChanged, bool, bIsVisible);
	FORCEINLINE FOnLoadingScreenVisibilityChanged& GetOnLoadingScreenVisibilityChangedDelegate(){return OnLoadingScreenVisibilityChanged;};


	void RegisterLoadingProcessor(TScriptInterface<IGINILoadingProcessInterface> LoadingProcessor);
	void UnregisterLoadingProcessor(TScriptInterface<IGINILoadingProcessInterface> LoadingProcessor);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenReasonChanged, const FString&, NewReason);
	UPROPERTY(BlueprintAssignable)
	FOnLoadingScreenReasonChanged OnLoadingScreenReasonChanged;
protected:
	UFUNCTION()
	void HandlePreLoadMap(const FWorldContext& WorldContext, const FString& String);
	UFUNCTION()
	void HandlePostLoadMap(UWorld* World);

	/* Determines if we should show or hide the loading screen Called every frame */
	void UpdateLoadingScreen();

	/* Returns true if there is any need to show the loading screen */
	bool CheckForAnyNeedToShowLoadingScreen();

	bool ShouldShowLoadingScreen();
	
	/* Returns true if we are in the initial loading flow before this screen should be used */
	bool IsShowingInitialLoadingScreen() const;

	void ShowLoadingScreen();

	void HideLoadingScreen();

	void RemoveWidgetFromViewport();

	void StartBlockingInput();

	void StopBlockingInput();

	void ChangePerformanceSettings(bool bEnablingLoadingScreen);

	void SetNewDebuggingLoadingReason(const FString& Reason);
	
	FString DebugReasonForLoadingScreen = FString("NONE");
	bool bIsLoadingScreenDisplayed = false;
	
	FOnLoadingScreenVisibilityChanged OnLoadingScreenVisibilityChanged;

	TSharedPtr<SWidget> LoadingScreenWidget;
	TSharedPtr<IInputProcessor> InputPreProcessor;

	TArray<TWeakInterfacePtr<IGINILoadingProcessInterface>> LoadingProcessors;
	/** The time when we started showing the loading screen */
	double TimeLoadingScreenShown = 0.0;

	/** The time the loading screen most recently wanted to be dismissed (might still be up due to a min display duration requirement) **/
	double TimeLoadingScreenLastDismissed = -1.0;

	/** The time until the next log for why the loading screen is still up */
	double TimeUntilNextLogHeartbeatSeconds = 0.0;

	/** True when we are between PreLoadMap and PostLoadMap */
	bool bCurrentlyInLoadMap = false;

	/** True when the loading screen is currently being shown */
	bool bCurrentlyShowingLoadingScreen = false;
};


