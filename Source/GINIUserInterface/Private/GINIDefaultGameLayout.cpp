// The Griffin Initiative


#include "GINIDefaultGameLayout.h"

#include "GINIUIManagerSubsystem.h"
#include "GINIUIPolicy.h"
#include "LogGINIUI.h"
#if WITH_EDITOR
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "WidgetBlueprint.h"
#endif
#include "CommonInputSubsystem.h"
#include "CommonUISubsystemBase.h"
#include "Components/Overlay.h"
#include "Foundation/GINIUITags.h"
#include "Kismet/GameplayStatics.h"



UGINIDefaultGameLayout* UGINIDefaultGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject)
{
	//Get the game instance:
	UGameInstance* const pGameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* const pPlayerController = pGameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayoutForPlayerController(pPlayerController);
	
}

UGINIDefaultGameLayout* UGINIDefaultGameLayout::GetPrimaryGameLayoutForPlayerController(const APlayerController* PlayerController)
{
	return PlayerController ? GetPrimaryGameLayoutForLocalPlayer(Cast<ULocalPlayer>(PlayerController->Player)) : nullptr;
}

UGINIDefaultGameLayout* UGINIDefaultGameLayout::GetPrimaryGameLayoutForLocalPlayer(const ULocalPlayer* LocalPlayer)
{
	if (!LocalPlayer)
	{
		return nullptr;
	}
	if (const UGameInstance* GameInstance = LocalPlayer->GetGameInstance())
	{
		if (UGINIUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UGINIUIManagerSubsystem>())
		{
			if (const UGINIUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (UGINIDefaultGameLayout* RootLayout = Policy->GetRootLayout(LocalPlayer))
				{
					return RootLayout;
				}
			}
		}
	}
	
	return nullptr;
}
UGINIDefaultGameLayout::UGINIDefaultGameLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UGINIDefaultGameLayout::SetIsDormant(bool bIsDormant)
{
	if (m_bIsDormant != bIsDormant)
	{
		const ULocalPlayer* LP = GetOwningLocalPlayer();
		const int32 PlayerId = LP ? LP->GetControllerId() : -1;
		const TCHAR* OldDormancyStr = m_bIsDormant ? TEXT("Dormant") : TEXT("Not-Dormant");
		const TCHAR* NewDormancyStr = bIsDormant ? TEXT("Dormant") : TEXT("Not-Dormant");
		const TCHAR* PrimaryPlayerStr = LP && LP->IsPrimaryPlayer() ? TEXT("[Primary]") : TEXT("[Non-Primary]");
		UE_LOG(LogGINIUI, Display, TEXT("%s PrimaryGameLayout Dormancy changed for [%d] from [%s] to [%s]"), PrimaryPlayerStr, PlayerId, OldDormancyStr, NewDormancyStr);

		m_bIsDormant = bIsDormant;
		OnIsDormantChanged();
	}
}

void UGINIDefaultGameLayout::RemoveWidgetFromLayer(UCommonActivatableWidget* InWidget, const FGameplayTag& LayerName)
{
	if (m_UILayers.Contains(LayerName))
	{
		m_UILayers[LayerName]->RemoveWidget(*InWidget);
	}
}

void UGINIDefaultGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* InWidget)
{
	// We're not sure what layer the widget is on so go searching.
	for (const auto& LayerKVP : m_UILayers)
	{
		LayerKVP.Value->RemoveWidget(*InWidget);
	}
}

void UGINIDefaultGameLayout::OnDisplayedMenuWidgetChanged(UCommonActivatableWidget* Widget)
{
	if (MenuStack->GetNumWidgets() > 0)
	{
		return;
	}
	OnMenuStackEmpty.Broadcast();
}

void UGINIDefaultGameLayout::OnDisplayedMenuModalWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget)
{
	if (MenuModalQueue->GetNumWidgets() > 0)
	{
		return;
	}
	OnMenuModalStackEmpty.Broadcast();
}

void UGINIDefaultGameLayout::OnDisplayedGameHudWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget)
{
	if (GameHudStack->GetNumWidgets() > 0)
	{
		return;
	}
	OnGameHudStackEmpty.Broadcast();
}

void UGINIDefaultGameLayout::OnDisplayedGameModalWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget)
{
	if (GameHudModalQueue->GetNumWidgets() > 0)
	{
		return;
	}
	OnGameHudStackEmpty.Broadcast();
}

void UGINIDefaultGameLayout::OnDisplayedSystemWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget)
{
	if (SystemQueue->GetNumWidgets() > 0)
	{
		return;
	}
	OnSystemQueueEmpty.Broadcast();
}

UCommonActivatableWidgetContainerBase* UGINIDefaultGameLayout::GetLayerWidget(const FGameplayTag& LayerTag) const
{
	return m_UILayers.FindRef(LayerTag);
}

void UGINIDefaultGameLayout::NativePreConstruct()
{
	Super::NativePreConstruct();
	RegisterDefaultLayers();
	if (!IsDesignTime())
	{
		if (MenuStack)
		{
			MenuStack->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedMenuWidgetChanged);
		}
		if (MenuModalQueue)
		{
			MenuModalQueue->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedMenuModalWidgetChanged);
		}
		if (GameHudStack)
		{
			GameHudStack->OnDisplayedWidgetChanged().AddUObject(this, &UGINIDefaultGameLayout::OnDisplayedGameHudWidgetChanged);
		}
		if (GameHudModalQueue)
		{
			GameHudModalQueue->OnDisplayedWidgetChanged().AddUObject(this, &UGINIDefaultGameLayout::OnDisplayedGameHudWidgetChanged);
		}
		if (SystemQueue)
		{
			SystemQueue->OnDisplayedWidgetChanged().AddUObject(this, &UGINIDefaultGameLayout::OnDisplayedGameHudWidgetChanged);
		}
	}
}

void UGINIDefaultGameLayout::NativeDestruct()
{
	if (!IsDesignTime())
	{
		if (MenuStack)
		{
			MenuStack->OnDisplayedWidgetChanged().RemoveAll(this);
		}
		if (MenuModalQueue)
		{
			MenuModalQueue->OnDisplayedWidgetChanged().RemoveAll(this);
		}
		if (GameHudStack)
		{
			GameHudStack->OnDisplayedWidgetChanged().RemoveAll(this);
		}
		if (GameHudModalQueue)
		{
			GameHudModalQueue->OnDisplayedWidgetChanged().RemoveAll(this);
		}
		if (SystemQueue)
		{
			SystemQueue->OnDisplayedWidgetChanged().RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

void UGINIDefaultGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->OnTransitioningChanged.AddUObject(this, &ThisClass::OnWidgetStackTransitioning);

		//LayerWidget->SetTransitionDuration(0.0f);
		m_UILayers.Add(LayerTag, LayerWidget);
	}
}

void UGINIDefaultGameLayout::OnIsDormantChanged()
{
	
}

void UGINIDefaultGameLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* InWidget,
	bool bIsTransitioning)
{
	if (bIsTransitioning)
	{
		//#ToDo: Add support to suspending input for the owning player here:
		//const FName SuspendToken = UCommonUIExtensions::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransion"));
		//m_SuspendInputTokens.Add(SuspendToken);
	}
	else
	{
		//if (ensure(m_SuspendInputTokens.Num() > 0))
		//{
			//const FName m_SuspendInputTokens = m_SuspendInputTokens.Pop();
			//UCommonUIExtensions::ResumeInputForPlayer(GetOwningLocalPlayer(), SuspendToken);
		//}
	}
}

bool UGINIDefaultGameLayout::HasAnyWidgetsOnAnyLayer()
{
	bool bHasAnyWidgetsOnAnyLayer = false;
	for (auto keyValuePair : m_UILayers)
	{
		if (keyValuePair.Value->GetNumWidgets() > 0)
		{
			bHasAnyWidgetsOnAnyLayer = true;
			break;
		}
	}
	return bHasAnyWidgetsOnAnyLayer;
}

void UGINIDefaultGameLayout::RegisterDefaultLayers_Implementation()
{
	if (bRegisterDefaultLayers)
	{
		if (MenuStack)
		{
			RegisterLayer(GINI_UI_GameplayTags::UI_Layer_Menu, MenuStack);
		}
		if (GameHudStack)
		{
			RegisterLayer(GINI_UI_GameplayTags::UI_Layer_Game_HUD, GameHudStack);
		}
		if (GameHudModalQueue)
		{
			RegisterLayer(GINI_UI_GameplayTags::UI_Layer_Game_Modal, GameHudModalQueue);
		}
		if (MenuModalQueue)
		{
			RegisterLayer(GINI_UI_GameplayTags::UI_Layer_Menu_Modal, MenuModalQueue);
		}
		if (SystemQueue)
		{
			RegisterLayer(GINI_UI_GameplayTags::UI_Layer_System, SystemQueue);
		}
	}
}
