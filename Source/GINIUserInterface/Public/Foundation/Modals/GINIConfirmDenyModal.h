// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GINIModalBase.h"
#include "GINIConfirmDenyModal.generated.h"


class UGINICoreButton;
/**
 * 
 */
UCLASS()
class GINIUSERINTERFACE_API UGINIConfirmDenyModal : public UGINIModalBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnConfirmation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDenial();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modals", meta=(BindWidget))
	TObjectPtr<UGINICoreButton> ConfirmButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modals", meta=(BindWidget))
	TObjectPtr<UGINICoreButton> DenyButton = nullptr;
	
};
