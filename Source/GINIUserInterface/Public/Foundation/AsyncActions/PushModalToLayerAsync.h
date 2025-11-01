// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/CancellableAsyncAction.h"
#include "Foundation/Modals/GINIModalBase.h"
#include "PushModalToLayerAsync.generated.h"

enum class EAsyncWidgetLayerState : uint8;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushSystemModalToLayerAsync, UGINIModalBase*, Widget);

UCLASS()
class GINIUSERINTERFACE_API UPushModalToLayerAsync : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:
	virtual void Cancel() override;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (WorldContext="WorldContextObject", BlueprintInternalUseOnly= true))
	static UPushModalToLayerAsync* PushSystemModal(APlayerController* OwningPlayer, UPARAM(meta = (AllowAbstract=false)) TSoftClassPtr<UGINIModalBase> InWidgetClass, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, const FText& ModalTitle, const FText& ModalBody, bool bSuspendInputUntilComplete = true);
	virtual void Activate() override;

	virtual void OnLoadUpdate(EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget);
	
	UPROPERTY(BlueprintAssignable)
	FPushSystemModalToLayerAsync AfterPush;
private:
	FGameplayTag LayerName;
	bool bSuspendInputUntilComplete = false;
	TWeakObjectPtr<APlayerController> OwningPlayerPtr;
	TSoftClassPtr<UGINIModalBase> WidgetClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;

	FText TitleText;
	FText BodyText;
};
