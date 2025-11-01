// The Griffin Initiative


#include "Utility/GINI_UIBlueprintFunctionLibrary.h"

#include "GINIDefaultGameLayout.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "CommonActivatableWidget.h"
#include "GINIUIManagerSubsystem.h"
#include "GINIUIPolicy.h"
#include "Engine/GameInstance.h"
#include "Utility/GINIUIUtilities.h"
#include "Widgets/CommonActivatableWidgetContainer.h"


ECommonInputType UGINI_UIBlueprintFunctionLibrary::GetOwningPlayerInputType(const UUserWidget* UserWidgetContext)
{
	return GINI_UI_Utilities::GetOwningPlayerInputType(UserWidgetContext);
}

bool UGINI_UIBlueprintFunctionLibrary::IsOwningPlayerUsingTouch(const UUserWidget* UserWidgetContext)
{
	return GINI_UI_Utilities::IsOwningPlayerUsingTouch(UserWidgetContext);
}

bool UGINI_UIBlueprintFunctionLibrary::IsOwningPlayerUsingGamepad(const UUserWidget* UserWidgetContext)
{
	return GINI_UI_Utilities::IsOwningPlayerUsingGamepad(UserWidgetContext);
}


UCommonActivatableWidget* UGINI_UIBlueprintFunctionLibrary::PushContentToLayer_ForPlayer(
	const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	return GINI_UI_Utilities::PushContentToLayer_ForPlayer(LocalPlayer, LayerName, WidgetClass);
}

void UGINI_UIBlueprintFunctionLibrary::PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer,
	FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	GINI_UI_Utilities::PushStreamedContentToLayer_ForPlayer(LocalPlayer, LayerName, WidgetClass);
}

void UGINI_UIBlueprintFunctionLibrary::PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	GINI_UI_Utilities::PopContentFromLayer(ActivatableWidget);
}

ULocalPlayer* UGINI_UIBlueprintFunctionLibrary::GetLocalPlayerFromController(APlayerController* PlayerController)
{
	return GINI_UI_Utilities::GetLocalPlayerFromController(PlayerController);
}

FName UGINI_UIBlueprintFunctionLibrary::SuspendInputForPlayer(APlayerController* PlayerController,
	FName SuspendReason)
{
	return GINI_UI_Utilities::SuspendInputForPlayer(PlayerController, SuspendReason);
}

void UGINI_UIBlueprintFunctionLibrary::ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken)
{
	GINI_UI_Utilities::ResumeInputForPlayer(PlayerController, SuspendToken);
}
