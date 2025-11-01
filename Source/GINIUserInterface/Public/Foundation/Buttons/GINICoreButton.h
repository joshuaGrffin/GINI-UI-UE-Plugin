// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Foundation/GINICoreButtonInterface.h"
#include "GINICoreButton.Generated.h"

class UGINICoreTextBlock;
class UCommonActionWidget;

UCLASS(Blueprintable)
class GINIUSERINTERFACE_API UGINICoreButton : public UCommonButtonBase, public IGINICoreButtonInterface
{

	GENERATED_BODY()

public:
	UGINICoreButton(const FObjectInitializer& ObjectInitializer);
	
	//Core Button Interface
	virtual UCommonTextBlock* GetTextBlock_Implementation() override;
	virtual void SetText_Implementation(const FText& Text) override;
	virtual void SetActivationInputAction_Implementation(const FDataTableRowHandle& NewTriggeringInputAction) override;
	virtual void SetEnhancedTriggeringInputAction_Implementation(UInputAction* NewTriggeringInputAction) override;
	//~CoreButtonInterface
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UI)
	FText ButtonCopyText = FText();
	UPROPERTY(BlueprintReadWrite, Category=UI, meta=(BindWidgetOptional))
	TObjectPtr<UGINICoreTextBlock> ButtonText = nullptr;
	
};
