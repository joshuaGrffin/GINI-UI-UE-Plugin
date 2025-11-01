// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Foundation/GINIActivatableWidget.h"
#include "GINIModalBase.generated.h"

class UCommonTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModalEvent, int32, ModalSelection);

UCLASS()
class GINIUSERINTERFACE_API UGINIModalBase : public UGINIActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetBodyText(const FText& InBodyText);

	UFUNCTION(BlueprintCallable)
	void SetTitleText(const FText& InTitleText);
	
protected:
	virtual bool NativeOnHandleBackAction() override;
	virtual void NativeConstruct() override;
	UGINIModalBase(const FObjectInitializer& InObjectInitializer);

#if WITH_EDITOR

	virtual void SynchronizeProperties() override;
	
#endif
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modals|Content")
	FText ModalBodyText = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modals|Content")
	FText ModalTitleText = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modals", meta =(BindWidgetOptional = true))
	TObjectPtr<UCommonTextBlock> ModalTitle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modals", meta =(BindWidgetOptional = true))
	TObjectPtr<UCommonTextBlock> ModalBody = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modals", meta = (ExposeOnSpawn))
	int32 OnClosedDefaultSelection = 0;
	
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable, Category = UI)
	FOnModalEvent OnModalSelection;
};
