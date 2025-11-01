// The Griffin Initiative


#include "Utility/GINIUIUtilities.h"

#include "CommonInputSubsystem.h"
#include "GINIDefaultGameLayout.h"
#include "GINIUIManagerSubsystem.h"
#include "GINIUIPolicy.h"
#include "Blueprint/UserWidget.h"

ECommonInputType GINI_UI_Utilities::GetOwningPlayerInputType(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType();
		}
	}

	return ECommonInputType::Count;
}

bool GINI_UI_Utilities::IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType() == ECommonInputType::Touch;
		}
	}
	return false;
}

bool GINI_UI_Utilities::IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;
		}
	}
	return false;
}

UCommonActivatableWidget* GINI_UI_Utilities::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer,
	FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(WidgetClass != nullptr))
	{
		return nullptr;
	}

	if (UGINIUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGINIUIManagerSubsystem>())
	{
		if (UGINIUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UGINIDefaultGameLayout* RootLayout = Policy->GetRootLayout(LocalPlayer))
			{
				return RootLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}

	return nullptr;
}

void GINI_UI_Utilities::PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName,
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(!WidgetClass.IsNull()))
	{
		return;
	}

	if (UGINIUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGINIUIManagerSubsystem>())
	{
		if (UGINIUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UGINIDefaultGameLayout* RootLayout = Policy->GetRootLayout(LocalPlayer))
			{
				constexpr bool bSuspendInputUntilComplete = true;
				RootLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
			}
		}
	}
}

void GINI_UI_Utilities::PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	if (!ActivatableWidget)
	{
		// Ignore request to pop an already deleted widget
		return;
	}

	if (const ULocalPlayer* LocalPlayer = ActivatableWidget->GetOwningLocalPlayer())
	{
		if (const UGINIUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGINIUIManagerSubsystem>())
		{
			if (const UGINIUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (UGINIDefaultGameLayout* RootLayout = Policy->GetRootLayout(LocalPlayer))
				{
					RootLayout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
				}
			}
		}
	}
}

void GINI_UI_Utilities::PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget, FGameplayTag LayerName)
{
	if (!ActivatableWidget)
	{
		// Ignore request to pop an already deleted widget
		return;
	}

	if (const ULocalPlayer* LocalPlayer = ActivatableWidget->GetOwningLocalPlayer())
	{
		if (const UGINIUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGINIUIManagerSubsystem>())
		{
			if (const UGINIUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (UGINIDefaultGameLayout* RootLayout = Policy->GetRootLayout(LocalPlayer))
				{
					RootLayout->RemoveWidgetFromLayer(ActivatableWidget, LayerName);
				}
			}
		}
	}
}

ULocalPlayer* GINI_UI_Utilities::GetLocalPlayerFromController(APlayerController* PlayerController)
{
	return Cast<ULocalPlayer>(PlayerController->Player);
}

FName GINI_UI_Utilities::SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason)
{
	return SuspendInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendReason);
}

FName GINI_UI_Utilities::SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		InputSuspensions++;
		FName SuspendToken = SuspendReason;
		SuspendToken.SetNumber(InputSuspensions);

		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, true);

		return SuspendToken;
	}

	return NAME_None;
}

void GINI_UI_Utilities::ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken)
{
	ResumeInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void GINI_UI_Utilities::ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken)
{
	if (SuspendToken == NAME_None)
	{
		return;
	}

	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, false);
	}
}
