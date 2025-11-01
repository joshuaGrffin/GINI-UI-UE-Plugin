// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/CancellableAsyncAction.h"
#include "PushContentToLayerAsync.generated.h"

/**
 * 
 */

enum class EAsyncWidgetLayerState : uint8;
class APlayerController;
class UCommonActivatableWidget;
class UObject;
struct FFrame;
struct FStreamableHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushContentToLayerAsync, UCommonActivatableWidget*, Widget);

UCLASS()
class GINIUSERINTERFACE_API UPushContentToLayerAsync : public UCancellableAsyncAction
{
	GENERATED_BODY()
public:
	virtual void Cancel() override;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (WorldContext="WorldContextObject", BlueprintInternalUseOnly= true))
	static UPushContentToLayerAsync* PushContentToLayerAsync(APlayerController* OwningPlayer, UPARAM(meta = (AllowAbstract=false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, bool bSuspendInputUntilComplete = true);
	virtual void Activate() override;

	virtual void OnLoadUpdate(EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget);
	
	UPROPERTY(BlueprintAssignable)
	FPushContentToLayerAsync BeforePush;
	UPROPERTY(BlueprintAssignable)
	FPushContentToLayerAsync AfterPush;
private:
	FGameplayTag LayerName;
	bool bSuspendInputUntilComplete = false;
	TWeakObjectPtr<APlayerController> OwningPlayerPtr;
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;
};
