// The Griffin Initiative


#include "LoadingScreen/GINILoadingScreenSubsystem.h"

#include "PreLoadScreenManager.h"
#include "ShaderPipelineCache.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Application/IInputProcessor.h"
#include "GameFramework/GameStateBase.h"
#include "HAL/ThreadHeartBeat.h"
#include "LoadingScreen/GINILoadingProcessInterface.h"
#include "LoadingScreen/GINILoadingScreenSettings.h"
#include "Widgets/Images/SThrobber.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLoadingScreen, Log, All);
DEFINE_LOG_CATEGORY(LogLoadingScreen);
CSV_DEFINE_CATEGORY(LoadingScreen, true);

namespace LoadingScreenCVars
{
	static float HoldLoadingScreenAdditionalSeconds = 2.0f;
	static FAutoConsoleVariable CVarHoldLoadingScreenUpForAtLeastThisLongInSeconds(
		TEXT("GINILoadingScreen.HoldLoadingScreenAdditionalSeconds"),
		HoldLoadingScreenAdditionalSeconds,
		TEXT("How long to hold the loading screen up after other loading finishes (in seconds)"),
		ECVF_Default|ECVF_Preview);

	static bool bLogLoadingScreenReasonEveryFrame = false;
	static FAutoConsoleVariable CVarLogLoadingScreenReasonEveryFrame(
		TEXT("GINILoadingScreen.LogLoadingScreenReasoneEveryFrame"),
		bLogLoadingScreenReasonEveryFrame,
		TEXT("When True, the reason the loading screen is shown or hidden will be printed to the log every frame, can be framie."),
		ECVF_Default);

	static bool bForceLodingScreenVisible = false;
	static FAutoConsoleVariable CVarForceLodingScreenVisible(
		TEXT("GINILoadingScreen.ForceLoadingScreenVisible"),
		bForceLodingScreenVisible,
		TEXT("Force the holding screen to be visible?"),
		ECVF_Default);
}

class FLoadingScreenPreProcessor : public IInputProcessor
{
public:
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool
	HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool
	HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent,
		const FPointerEvent* InGestureEvent) override;
	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override;

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {};

	FLoadingScreenPreProcessor() = default;
	virtual ~FLoadingScreenPreProcessor() = default;
	bool CanEatInput() const
	{
		return !GIsEditor;
	}
	
	
};

bool FLoadingScreenPreProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleAnalogInputEvent(FSlateApplication& SlateApp,
	const FAnalogInputEvent& InAnalogInputEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp,
	const FPointerEvent& MouseEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp,
	const FPointerEvent& MouseEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp,
	const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent)
{
	return CanEatInput();
}

bool FLoadingScreenPreProcessor::HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent)
{
	return CanEatInput();
}


bool IGINILoadingProcessInterface::ShouldShowLoadingScreen(UObject* TestObject, FString& OutMessage)
{
	if (TestObject)
	{
		if (IGINILoadingProcessInterface* LoadObserver = Cast<IGINILoadingProcessInterface>(TestObject))
		{
			FString ObserverReason;
			if (LoadObserver->ShouldShowLoadingScreenReason_Implementation(ObserverReason))
			{
				if (ensureMsgf(!ObserverReason.IsEmpty(), TEXT("%s Failed to set a reason why it wants to show the loading screen"), *GetPathNameSafe(TestObject)))
				{
					OutMessage = ObserverReason;
				}
				return true;
			}
		}
	}
	return false;
}


void UGINILoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::HandlePostLoadMap);
	const UGameInstance* GameInstance = GetGameInstance();
	check(GameInstance);
}

void UGINILoadingScreenSubsystem::Deinitialize()
{
	StopBlockingInput();
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

bool UGINILoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	const bool bIsServer = GameInstance->IsDedicatedServerInstance();
	return !bIsServer;
}

void UGINILoadingScreenSubsystem::Tick(float DeltaTime)
{
	UpdateLoadingScreen();
	TimeUntilNextLogHeartbeatSeconds = FMath::Max(TimeUntilNextLogHeartbeatSeconds - DeltaTime, 0.f);
}

bool UGINILoadingScreenSubsystem::IsTickable() const
{
	return !HasAnyFlags(RF_ClassDefaultObject);
}

ETickableTickType UGINILoadingScreenSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

TStatId UGINILoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULoadingScreenSubsystem, STATGROUP_Tickables);
}

UWorld* UGINILoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	return  GetGameInstance()->GetWorld();
}

void UGINILoadingScreenSubsystem::RegisterLoadingProcessor(
	TScriptInterface<IGINILoadingProcessInterface> LoadingProcessor)
{
	LoadingProcessors.AddUnique(LoadingProcessor.GetObject());
}

void UGINILoadingScreenSubsystem::UnregisterLoadingProcessor(
	TScriptInterface<IGINILoadingProcessInterface> LoadingProcessor)
{
	LoadingProcessors.Remove(LoadingProcessor.GetObject());
}

void UGINILoadingScreenSubsystem::HandlePreLoadMap(const FWorldContext& WorldContext, const FString& String)
{
	if (WorldContext.OwningGameInstance == GetGameInstance())
	{
		bCurrentlyInLoadMap = true;
		if (GEngine->IsInitialized())
		{
			UpdateLoadingScreen();
		}
	}
}

void UGINILoadingScreenSubsystem::HandlePostLoadMap(UWorld* World)
{
	if (World != nullptr && World->GetGameInstance() == GetGameInstance())
	{
		bCurrentlyInLoadMap = false;
	}
}

void UGINILoadingScreenSubsystem::UpdateLoadingScreen()
{
	bool bLogLoadingScreen = LoadingScreenCVars::bLogLoadingScreenReasonEveryFrame;

	if (ShouldShowLoadingScreen())
	{
		//Get the settings
		const UGINILoadingScreenSettings* LoadingScreenSettings = GetDefault<UGINILoadingScreenSettings>(); 
		//if we dont make it to the specified checkpoint in the given time, we can trigger a hang detector to see where we are stalling.
		FThreadHeartBeat::Get().MonitorCheckpointStart(GetFName(), LoadingScreenSettings->LoadingScreenHeartbeatHangDuration);

		ShowLoadingScreen();

		if (LoadingScreenSettings->LogLoadingScreenHeartbeatInterval >0.0f && TimeUntilNextLogHeartbeatSeconds > 0.0f)
		{
			bLogLoadingScreen = true;
			TimeUntilNextLogHeartbeatSeconds = LoadingScreenSettings->LogLoadingScreenHeartbeatInterval;
		}
	}
	else
	{
		HideLoadingScreen();
		FThreadHeartBeat::Get().MonitorCheckpointEnd(GetFName());
	}

	if (bLogLoadingScreen)
	{
		UE_LOG(LogLoadingScreen, Log, TEXT("Loading Screen Showing: %d, Reason: %s"), bCurrentlyShowingLoadingScreen, *DebugReasonForLoadingScreen);
	}
}

bool UGINILoadingScreenSubsystem::CheckForAnyNeedToShowLoadingScreen()
{
	SetNewDebuggingLoadingReason(TEXT("Reason for Showing/Hiding LoadingScreen is unknown!"));

	const UGameInstance* GameInstance = GetGameInstance();

	if (LoadingScreenCVars::bForceLodingScreenVisible)
	{
		SetNewDebuggingLoadingReason(TEXT("CommonLoadingScreen.AlwaysShow is true"));
		return true;
	}

	const FWorldContext* const WorldContext = GameInstance->GetWorldContext();

	if (WorldContext == nullptr)
	{
		SetNewDebuggingLoadingReason(TEXT("We have no world context from the game instance"));
		return true;
	}

	const UWorld* World =  WorldContext->World();
	if (World == nullptr)
	{
		SetNewDebuggingLoadingReason(TEXT("We have no world! (FWorldContext World() is null)"));
		return true;
	}
	
	AGameStateBase* GameState = World->GetGameState();
	if (GameState == nullptr)
	{
		SetNewDebuggingLoadingReason(TEXT("We have no game state!"));
		return true;
	}

	if (bCurrentlyInLoadMap)
	{
		SetNewDebuggingLoadingReason(TEXT("bCurrentlyInLoadMap is true"));
		return true;
	}

	if (!WorldContext->TravelURL.IsEmpty())
	{
		SetNewDebuggingLoadingReason(TEXT("TravelURL is not empty, we are pending travel!"));
		return true;
	}

	if (WorldContext->PendingNetGame != nullptr)
	{
		SetNewDebuggingLoadingReason(TEXT("We are pending server connection! Pending A Net Game!"));
		return true;
	}

	if (!World->HasBegunPlay())
	{
		SetNewDebuggingLoadingReason(TEXT("We have not started play yet!"));
		return true;
	}

	if (World->IsInSeamlessTravel())
	{
		SetNewDebuggingLoadingReason(TEXT("We are traveling."));
		return true;
	}

	//Ask the Game state if we need a loading screen: 
	if (IGINILoadingProcessInterface::ShouldShowLoadingScreen(GameState, DebugReasonForLoadingScreen))
	{
		return true;
	}

	//Ask the game state components if they have a reason to show the loading screen: 
	for (TObjectPtr<UActorComponent> Component : GameState->GetComponents())
	{
		//Ask it if it needs to be shown
		if (IGINILoadingProcessInterface::ShouldShowLoadingScreen(Component, DebugReasonForLoadingScreen))
		{
			OnLoadingScreenReasonChanged.Broadcast(DebugReasonForLoadingScreen);
			return true;
		}
	}

	for (const TWeakInterfacePtr<IGINILoadingProcessInterface>& Processor : LoadingProcessors)
	{
		if (IGINILoadingProcessInterface::ShouldShowLoadingScreen(Processor.GetObject(), DebugReasonForLoadingScreen))
		{
			OnLoadingScreenReasonChanged.Broadcast(DebugReasonForLoadingScreen);
			return true;
		}
	}
	
	bool bFoundAnyLocalPC = false;
	bool bMissingAnyLocalPC = false;

	for (ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers())
	{
		if (LocalPlayer != nullptr)
		{
			if (APlayerController* PC = LocalPlayer->PlayerController)
			{
				bFoundAnyLocalPC = true;
				//Ask the Player Controller if there is a reason to show the loading screen

				if (IGINILoadingProcessInterface::ShouldShowLoadingScreen(PC, DebugReasonForLoadingScreen))
				{
					OnLoadingScreenReasonChanged.Broadcast(DebugReasonForLoadingScreen);
					return true;
				}
				
				//Ask components for any reasons:
				for (UActorComponent* Component : PC->GetComponents())
				{
					if (IGINILoadingProcessInterface::ShouldShowLoadingScreen(Component, DebugReasonForLoadingScreen))
					{
						OnLoadingScreenReasonChanged.Broadcast(DebugReasonForLoadingScreen);
						return true;
					}
				}
				
			}
		}
		else
		{
			bMissingAnyLocalPC = true;	
		}
	}
	

	UGameViewportClient* GameViewportClient = GameInstance->GetGameViewportClient();
	const bool bIsInSplitScreen = GameViewportClient->GetCurrentSplitscreenConfiguration() != ESplitScreenType::None;

	if (bIsInSplitScreen && bMissingAnyLocalPC)
	{
		SetNewDebuggingLoadingReason(TEXT("We are missing a player controller for split screen!"));
		return true;
	}

	if (!bIsInSplitScreen && !bFoundAnyLocalPC)
	{
		SetNewDebuggingLoadingReason(TEXT("We need at least 1 local player controller for Split Screen"));
		return true;
	}
	
	SetNewDebuggingLoadingReason(TEXT("(Nothing requires a loading screen anymore!)"));
	return false;
}

bool UGINILoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	// Get the loading Screen Settings

	const UGINILoadingScreenSettings* LoadingScreenSettings = GetDefault<UGINILoadingScreenSettings>();
	
#if !UE_BUILD_SHIPPING
	static bool bCmdLineNoLoadingScreen = FParse::Param(FCommandLine::Get(), TEXT("NoLoadingScreen"));
	if (bCmdLineNoLoadingScreen)
	{
		DebugReasonForLoadingScreen = TEXT("Commandline has NoLoadingScreen");
		return false;
	}
#endif

	const bool bNeedToShowLoadingScreen = CheckForAnyNeedToShowLoadingScreen();

	bool bWantToForceShowLaodingScreen = false;
	if (bNeedToShowLoadingScreen)
	{
		TimeLoadingScreenLastDismissed = -1.0f;
	}
	else
	{
		const double currentTime = FPlatformTime::Seconds();
		//Get an override from the settings for editor seconds 
		const bool bCanHoldLoadingScreen = (!GIsEditor || LoadingScreenSettings->bHoldLoadingScreenAdditionalSecondsInEditor);
		const double HoldLoadingScreenTime = bCanHoldLoadingScreen ? LoadingScreenCVars::HoldLoadingScreenAdditionalSeconds : 0.0;

		if (TimeLoadingScreenLastDismissed < 0.0)
		{
			TimeLoadingScreenLastDismissed = currentTime;
		}

		const double TimeSinceDismissed = currentTime - TimeLoadingScreenLastDismissed;

		if (HoldLoadingScreenTime > 0.0 && TimeSinceDismissed < HoldLoadingScreenTime)
		{
			//#TODO: if we turn the loading screen back on in this time frame, we wont turn off the world rendering:
			UGameViewportClient* GameViewportClient = GetGameInstance()->GetGameViewportClient();
			GameViewportClient->bDisableWorldRendering = false;

			SetNewDebuggingLoadingReason(FString::Printf(TEXT("Keeping the loading screen up for some additional time %.2f to allow texture streaming "), HoldLoadingScreenTime));
			bWantToForceShowLaodingScreen = true;
		}
	}
	return bNeedToShowLoadingScreen || bWantToForceShowLaodingScreen;
}

bool UGINILoadingScreenSubsystem::IsShowingInitialLoadingScreen() const
{
	FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get();
	return PreLoadScreenManager != nullptr && PreLoadScreenManager->HasValidActivePreLoadScreen();

}

void UGINILoadingScreenSubsystem::ShowLoadingScreen()
{
	if (bCurrentlyShowingLoadingScreen)
	{
		return; 
	}

	//Check for the Preloading screen if the engine is still loading with its loading screen
	if (FPreLoadScreenManager::Get() && FPreLoadScreenManager::Get()->HasActivePreLoadScreenType(EPreLoadScreenTypes::EngineLoadingScreen))
	{
		return;
	}

	TimeLoadingScreenShown = FPlatformTime::Seconds();

	bCurrentlyShowingLoadingScreen = true;

	CSV_EVENT(LoadingScreen, TEXT("Show"));
	
	if (IsShowingInitialLoadingScreen())
	{
		UE_LOG(LogLoadingScreen, Display, TEXT("Showing Loading screen when 'ShowingInitialLoadingScreen' is true \n %s"), *DebugReasonForLoadingScreen);
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	StartBlockingInput();
	OnLoadingScreenVisibilityChanged.Broadcast(true);

	const UGINILoadingScreenSettings* LoadingScreenSettings = GetDefault<UGINILoadingScreenSettings>();
	
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass = LoadingScreenSettings->LoadingScreenClassPath.TryLoadClass<UUserWidget>();
	if (UUserWidget* UserWidget = UUserWidget::CreateWidgetInstance(*GameInstance, LoadingScreenWidgetClass,NAME_None))
	{
		LoadingScreenWidget = UserWidget->TakeWidget();
	}
	else
	{
		UE_LOG(LogLoadingScreen, Error, TEXT("Failed to load Loading Screen widget %s, falling back to placeholder"), *LoadingScreenSettings->LoadingScreenClassPath.ToString());
		LoadingScreenWidget = SNew(SThrobber);
	}

	UGameViewportClient* GameViewportClient = GetGameInstance()->GetGameViewportClient();
	GameViewportClient->AddViewportWidgetContent(LoadingScreenWidget.ToSharedRef(), LoadingScreenSettings->LoadingScreenZOrder);

	ChangePerformanceSettings(true);


	if (!GIsEditor || LoadingScreenSettings->bForceLoadingScreenVisible)
	{
		FSlateApplication::Get().Tick();
	}
	
}


void UGINILoadingScreenSubsystem::HideLoadingScreen()
{
	if (!bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	StopBlockingInput();

	if (IsShowingInitialLoadingScreen())
	{
		UE_LOG(LogLoadingScreen, Display, TEXT("Hiding Loading screen when 'ShowingInitialLoadingScreen' is true \n %s"), *DebugReasonForLoadingScreen);
		return;
	}

	UE_LOG(LogLoadingScreen, Log, TEXT("Garbage Collecting before dropping the loading screen"));

	//Can cause a hitch, but good here: 
	GEngine->ForceGarbageCollection(true);

	RemoveWidgetFromViewport();

	ChangePerformanceSettings(false);
	OnLoadingScreenVisibilityChanged.Broadcast(false);

	CSV_EVENT(LoadingScreen, TEXT("Hiding"));

	const double LoadingScreenDuration = FPlatformTime::Seconds() - TimeLoadingScreenShown;
	UE_LOG(LogLoadingScreen, Log, TEXT("Time Shown %.2f"), LoadingScreenDuration);

	bCurrentlyShowingLoadingScreen = false;
}

void UGINILoadingScreenSubsystem::RemoveWidgetFromViewport()
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	if (LoadingScreenWidget.IsValid())
	{
		if (UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient())
		{
			GameViewportClient->RemoveViewportWidgetContent(LoadingScreenWidget.ToSharedRef());
		}
		LoadingScreenWidget.Reset();
	}
}

void UGINILoadingScreenSubsystem::StartBlockingInput()
{
	if (!InputPreProcessor.IsValid())
	{
		InputPreProcessor = MakeShareable<FLoadingScreenPreProcessor>(new FLoadingScreenPreProcessor());
		FSlateApplication::Get().RegisterInputPreProcessor(InputPreProcessor, 0);
	}
}

void UGINILoadingScreenSubsystem::StopBlockingInput()
{
	if (InputPreProcessor.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputPreProcessor);
		InputPreProcessor.Reset();
	}
}

void UGINILoadingScreenSubsystem::ChangePerformanceSettings(bool bEnablingLoadingScreen)
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient();

	FShaderPipelineCache::SetBatchMode(bEnablingLoadingScreen ? FShaderPipelineCache::BatchMode::Fast : FShaderPipelineCache::BatchMode::Background);

	GameViewportClient->bDisableWorldRendering = bEnablingLoadingScreen;

	if (UWorld* World = GameViewportClient->GetWorld())
	{
		if (AWorldSettings* WorldSettings = World->GetWorldSettings())
		{
			WorldSettings->bHighPriorityLoading = bEnablingLoadingScreen;
		}
	}

	if (bEnablingLoadingScreen)
	{
		double HangDurationMultiplier = 0.0;
		if (!GConfig || !GConfig->GetDouble(TEXT("Core.System"), TEXT("LoadingScreenHangDurationMultiplier"), HangDurationMultiplier, GEngineIni))
		{
			HangDurationMultiplier = 1.0;
		}

		FThreadHeartBeat::Get().SetDurationMultiplier(HangDurationMultiplier);

		//We dont really care about hitches when the loading screen is up, so suppress them 
		FGameThreadHitchHeartBeat::Get().SuspendHeartBeat();
	}
	else
	{
		FThreadHeartBeat::Get().SetDurationMultiplier(1.0);

		FGameThreadHitchHeartBeat::Get().ResumeHeartBeat();
	}
	
	
}
void UGINILoadingScreenSubsystem::SetNewDebuggingLoadingReason(const FString& Reason)
{
	DebugReasonForLoadingScreen = Reason;
	//#TODO: Change this to being a FTEXT for Localization Reasons
	OnLoadingScreenReasonChanged.Broadcast(Reason);
}





