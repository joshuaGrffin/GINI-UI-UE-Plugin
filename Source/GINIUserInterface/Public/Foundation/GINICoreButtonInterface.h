// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GINICoreButtonInterface.generated.h"

class UCommonTextBlock;
class UInputAction;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGINICoreButtonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GINIUSERINTERFACE_API IGINICoreButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCommonTextBlock* GetTextBlock();
	virtual UCommonTextBlock* GetTextBlock_Implementation() = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetText(const FText& Text);
	virtual void SetText_Implementation(const FText& Text) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetActivationInputAction(const FDataTableRowHandle& NewTriggeringInputAction);
	virtual void SetActivationInputAction_Implementation(const FDataTableRowHandle& NewTriggeringInputAction) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetEnhancedTriggeringInputAction(UInputAction* NewTriggeringInputAction);
	virtual void SetEnhancedTriggeringInputAction_Implementation(UInputAction* NewTriggeringInputAction) = 0;
	
};
