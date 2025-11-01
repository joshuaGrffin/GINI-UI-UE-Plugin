// The Griffin Initiative


#include "GINIUIPolicy.h"

#include "LogGINIUI.h"
#include "GINIUIManagerSubsystem.h"

FRootLayoutInfo::FRootLayoutInfo(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout, bool bIsInViewport)
	: LocalPlayer(LocalPlayer), GameLayout(GameLayout), bAddedToViewport(bIsInViewport)
{
}

UGINIUIPolicy* UGINIUIPolicy::GetUIPolicy(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	UGINIUIManagerSubsystem* UIManagerSubsystem = GameInstance->GetSubsystem<UGINIUIManagerSubsystem>();
	if (!UIManagerSubsystem)
	{
		return nullptr;
	}

	return UIManagerSubsystem->GetCurrentUIPolicy();
}

UWorld* UGINIUIPolicy::GetWorld() const
{
	return GetUIManagerSubsystem()->GetGameInstance()->GetWorld();
}

UGINIUIManagerSubsystem* UGINIUIPolicy::GetUIManagerSubsystem() const
{
	return CastChecked<UGINIUIManagerSubsystem>(GetOuter());
}

UGINIDefaultGameLayout* UGINIUIPolicy::GetRootLayout(const ULocalPlayer* LocalPlayer) const
{
	const FRootLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->GameLayout : nullptr;
}

ELocalMultiplayerInteractionMode UGINIUIPolicy::GetLocalMultiplayerInteractionMode() const
{
	return LocalMultiplayerInteractionMode;
}

void UGINIUIPolicy::RequestControl(UGINIDefaultGameLayout* GameLayout)
{
	if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && GameLayout->IsDormant())
	{
		for (const FRootLayoutInfo& LayoutInfo : RootViewportLayouts)
		{
			UGINIDefaultGameLayout* RootLayout = LayoutInfo.GameLayout;
			if (RootLayout && !RootLayout->IsDormant())
			{
				RootLayout->SetIsDormant(true);
				break;
			}
		}
		GameLayout->SetIsDormant(false);
	}
}

void UGINIUIPolicy::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout)
{
	UE_LOG(LogGINIUI, Log, TEXT("%s is adding player %s's root layout %s to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(GameLayout));

	GameLayout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	GameLayout->AddToPlayerScreen(1000);
	OnRootLayoutAddedToViewport(LocalPlayer, GameLayout);
}

void UGINIUIPolicy::RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = GameLayout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogGINIUI, Log, TEXT("%s is removing player %s's root layout %s from the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(GameLayout));

		GameLayout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogGINIUI, Log, TEXT("Player %s's root layout %s has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."), *GetNameSafe(LocalPlayer), *GetNameSafe(GameLayout));
		}

		OnRootLayoutRemovedFromViewport(LocalPlayer, GameLayout);
	}
}

void UGINIUIPolicy::OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout)
{
#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
}

void UGINIUIPolicy::OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout)
{
	
}

void UGINIUIPolicy::OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout)
{
	
}

void UGINIUIPolicy::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UGINIDefaultGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UGINIDefaultGameLayout* NewLayoutObject = CreateWidget<UGINIDefaultGameLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);
			
			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UGINIDefaultGameLayout> UGINIUIPolicy::GetLayoutWidgetClass(ULocalPlayer* LocalPlayer) const
{
	return LayoutClass.LoadSynchronous();
}

void UGINIUIPolicy::OnLocalPlayerAdded(ULocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerChanged().AddWeakLambda(this, [this](APlayerController* PlayerController)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
		OnLocalPlayerRemoved(LocalPlayer);

		if (FRootLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
		{
			AddLayoutToViewport(LocalPlayer, LayoutInfo->GameLayout);
			LayoutInfo->bAddedToViewport = true;
		}
		else
		{
			CreateLayoutWidget(LocalPlayer);
		}
	});

	if (FRootLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->GameLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGINIUIPolicy::OnLocalPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (FRootLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->GameLayout);
		LayoutInfo->bAddedToViewport = false;

		if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && !LocalPlayer->IsPrimaryPlayer())
		{
			UGINIDefaultGameLayout* RootLayout = LayoutInfo->GameLayout;
			if (RootLayout && !RootLayout->IsDormant())
			{
				// We're removing a secondary player's root while it's in control - transfer control back to the primary player's root
				RootLayout->SetIsDormant(true);
				for (const FRootLayoutInfo& RootLayoutInfo : RootViewportLayouts)
				{
					if (RootLayoutInfo.LocalPlayer->IsPrimaryPlayer())
					{
						if (UGINIDefaultGameLayout* PrimaryRootLayout = RootLayoutInfo.GameLayout)
						{
							PrimaryRootLayout->SetIsDormant(false);
						}
					}
				}
			}
		}
	}
}

void UGINIUIPolicy::OnLocalPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	OnLocalPlayerRemoved(LocalPlayer);
	LocalPlayer->OnPlayerControllerChanged().RemoveAll(this);
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		UGINIDefaultGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].GameLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);

		RemoveLayoutFromViewport(LocalPlayer, Layout);

		OnRootLayoutReleased(LocalPlayer, Layout);
	}
}
