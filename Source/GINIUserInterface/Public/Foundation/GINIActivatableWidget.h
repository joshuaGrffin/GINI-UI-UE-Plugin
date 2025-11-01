// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GINIActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EInputConfig : uint8
{
	Default = 0,
	GameOnly = 1,
	MenuOnly = 2,
	GameAndMenu = 3
};

UCLASS()
class GINIUSERINTERFACE_API UGINIActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public: 
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	UFUNCTION(BlueprintPure)
	bool GetShouldHideMouseOnCapture() const
	{return bShouldHideMouseOnCapture;};
	
	UFUNCTION(BlueprintCallable)
	virtual void SetShouldHideMouseOnCapture(bool bHideMouseOnCapture)
	{bShouldHideMouseOnCapture = bHideMouseOnCapture;};
	
#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
	
protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(AllowPrivateAccess=true))
	EInputConfig InputConfig = EInputConfig::Default;

	UPROPERTY(BlueprintGetter="GetShouldHideMouseOnCapture", BlueprintSetter="SetShouldHideMouseOnCapture", EditAnywhere, meta=(AllowPrivateAccess=true))
	bool bShouldHideMouseOnCapture = true;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EMouseCaptureMode GameCaptureMode = EMouseCaptureMode::CapturePermanently;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EMouseCaptureMode UICaptureMode = EMouseCaptureMode::NoCapture;
};

inline TOptional<FUIInputConfig> UGINIActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EInputConfig::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameCaptureMode, bShouldHideMouseOnCapture);
	case EInputConfig::MenuOnly:
		 return FUIInputConfig(ECommonInputMode::Menu, UICaptureMode, bShouldHideMouseOnCapture);
	case EInputConfig::GameOnly:
		return FUIInputConfig(ECommonInputMode::Game, GameCaptureMode, bShouldHideMouseOnCapture);
	case EInputConfig::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
