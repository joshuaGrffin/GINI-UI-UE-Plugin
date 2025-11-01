// The Griffin Initiative


#include "GINIUIManagerSubsystem.h"

#include "GINIUIPolicy.h"
#include "Developer/ShaderCompilerCommon/Private/HlslLexer.h"
#include "GameFramework/HUD.h"
#include "Settings/GINIUISettings.h"

void UGINIUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (CurrentPolicy != nullptr)
	{
		return; 
	}
	const UUIManagerSettings* Settings = GetDefault<UUIManagerSettings>();

	DefaultUIPolicy = Settings->DefaultUIPolicy;
	bShouldSyncWithPlayerHUDVisibility = Settings->bShouldSyncWithPlayerHUDVisibility;
	
	if (!ensureMsgf(!DefaultUIPolicy.IsNull(), TEXT("The Default UI Policy is null, please update this as soon as possible")))
	{
		return;
	}


	
	TSubclassOf<UGINIUIPolicy> UIPolicyClass = DefaultUIPolicy.LoadSynchronous();
	SwitchToPolicy(NewObject<UGINIUIPolicy>(this, UIPolicyClass));

	GetGameInstance()->OnLocalPlayerAddedEvent.AddUObject(this, &UGINIUIManagerSubsystem::NotifyPlayerAdded);
	GetGameInstance()->OnLocalPlayerRemovedEvent.AddUObject(this, &UGINIUIManagerSubsystem::NotifyPlayerDestroyed);
	
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::Tick), 0.0f);
}

void UGINIUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		GameInstance->OnLocalPlayerAddedEvent.RemoveAll(this);
		GameInstance->OnLocalPlayerRemovedEvent.RemoveAll(this);
	}
	SwitchToPolicy(nullptr);
	FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
}

bool UGINIUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
}

void UGINIUIManagerSubsystem::NotifyPlayerAdded(ULocalPlayer* NewPlayer)
{
	if (ensure(NewPlayer) && CurrentPolicy)
	{
		CurrentPolicy->OnLocalPlayerAdded(NewPlayer);
	}
}

void UGINIUIManagerSubsystem::NotifyPlayerRemoved(ULocalPlayer* OldPlayer)
{
	if (ensure(OldPlayer) && CurrentPolicy)
	{
		CurrentPolicy->OnLocalPlayerRemoved(OldPlayer);
	}
}

void UGINIUIManagerSubsystem::NotifyPlayerDestroyed(ULocalPlayer* OldPlayer)
{
	if (ensure(OldPlayer) && CurrentPolicy)
	{
		CurrentPolicy->OnLocalPlayerDestroyed(OldPlayer);
	}
}

void UGINIUIManagerSubsystem::OnCinematicBegin()
{
	if (bShouldSyncWithPlayerHUDVisibility)
	{
		return;
	}
	//Get all players
	//Hide the layout widget
	const TArray<ULocalPlayer*>& LocalPlayers = GetGameInstance()->GetLocalPlayers();
	for (const ULocalPlayer* LocalPlayer : LocalPlayers)
	{
		UGINIDefaultGameLayout* Layout = UGINIDefaultGameLayout::GetPrimaryGameLayoutForLocalPlayer(LocalPlayer);
		if (Layout)
		{
			Layout->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGINIUIManagerSubsystem::OnCinematicEnd()
{
	if (bShouldSyncWithPlayerHUDVisibility)
	{
		return;
	}
	
	//Get all players
	//Show the layout widget
	const TArray<ULocalPlayer*>& LocalPlayers = GetGameInstance()->GetLocalPlayers();
	for (const ULocalPlayer* LocalPlayer : LocalPlayers)
	{
		UGINIDefaultGameLayout* Layout = UGINIDefaultGameLayout::GetPrimaryGameLayoutForLocalPlayer(LocalPlayer);
		if (Layout)
		{
			Layout->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGINIUIManagerSubsystem::SwitchToPolicy(UGINIUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

bool UGINIUIManagerSubsystem::Tick(float DeltaTime)
{
	SyncRootLayoutVisibilityToShowHUD();
	return true;
}

void UGINIUIManagerSubsystem::SyncRootLayoutVisibilityToShowHUD()
{
	if (bShouldSyncWithPlayerHUDVisibility && CurrentPolicy != nullptr )
	{
		for (const ULocalPlayer* const LocalPlayer : GetGameInstance()->GetLocalPlayers())
		{
			bool bShouldShowUI = true;

			if (const APlayerController* const PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
			{
				const AHUD* const HUD = PlayerController->GetHUD();
				if (HUD != nullptr && !HUD->bShowHUD)
				{
					bShouldShowUI = false;
				}
			}

			if (UGINIDefaultGameLayout* RootLayout = CurrentPolicy->GetRootLayout(LocalPlayer))
			{
				const ESlateVisibility DesiredVisibility = bShouldShowUI ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
				if (DesiredVisibility != RootLayout->GetVisibility())
				{
					RootLayout->SetVisibility(DesiredVisibility);
				}
			}
		}
	}
}
