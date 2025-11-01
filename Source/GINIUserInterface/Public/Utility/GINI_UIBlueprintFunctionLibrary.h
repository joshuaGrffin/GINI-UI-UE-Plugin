// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GINIUIUtilities.h"
#include "UObject/Object.h"
#include "GINI_UIBlueprintFunctionLibrary.generated.h"

enum class ECommonInputType : uint8;
template <typename T> class TSubclassOf;

class APlayerController;
class UCommonActivatableWidget;
class ULocalPlayer;
class UObject;
class UUserWidget;
struct FFrame;
struct FGameplayTag;

UCLASS()
class GINIUSERINTERFACE_API UGINI_UIBlueprintFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	//input Utility functions: 
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="UI Utilities", meta = (WorldContext = "UserWidgetContext"))
	static ECommonInputType GetOwningPlayerInputType(const UUserWidget* UserWidgetContext);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="UI Utilities", meta = (WorldContext = "UserWidgetContext"))
	static bool IsOwningPlayerUsingTouch(const UUserWidget* UserWidgetContext);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="UI Utilities", meta = (WorldContext = "UserWidgetContext"))
	static bool IsOwningPlayerUsingGamepad(const UUserWidget* UserWidgetContext);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Utilities")
	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Utilities")
	static void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Utilities")
	static void PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Utilities")
	static ULocalPlayer* GetLocalPlayerFromController(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);
	
};