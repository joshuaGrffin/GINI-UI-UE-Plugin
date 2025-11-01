// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"

struct FGameplayTag;
enum class ECommonInputType : uint8;
class UCommonActivatableWidget;
class APlayerController;
class ULocalPlayer;

namespace GINI_UI_Utilities
{
	ECommonInputType GetOwningPlayerInputType(const UUserWidget* WidgetContextObject);
	bool IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject);
	bool IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject);
	UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass);
	void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass);
	void PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget);
	void PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget, FGameplayTag LayerName);
	ULocalPlayer* GetLocalPlayerFromController(APlayerController* PlayerController);
	FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);
	FName SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason);
	void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);
	void ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken);

	inline int32 InputSuspensions = 0;
}
