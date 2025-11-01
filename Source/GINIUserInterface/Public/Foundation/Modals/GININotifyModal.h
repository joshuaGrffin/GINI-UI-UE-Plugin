// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GINIModalBase.h"
#include "GININotifyModal.generated.h"

/**
 * 
 */
UCLASS()
class GINIUSERINTERFACE_API UGININotifyModal : public UGINIModalBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnConfirmation();

	UPROPERTY(BlueprintReadOnly, Category="Modal", meta=(BindWidget))
	TObjectPtr<class UGINICoreButton> ConfirmationButton = nullptr;

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
};
