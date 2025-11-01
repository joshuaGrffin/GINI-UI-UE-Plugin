// The Griffin Initiative


#include "Foundation/Buttons/GINICoreButton.h"

#include "IMediaControls.h"
#include "Foundation/GINICoreTextBlock.h"

UGINICoreButton::UGINICoreButton(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

UCommonTextBlock* UGINICoreButton::GetTextBlock_Implementation()
{
	return ButtonText;
}

void UGINICoreButton::SetText_Implementation(const FText& Text)
{
	if (ButtonText)
	{
		ButtonText->SetText(Text);
	}
}
void UGINICoreButton::SetActivationInputAction_Implementation(const FDataTableRowHandle& NewTriggeringInputAction)
{
	SetTriggeringInputAction(NewTriggeringInputAction);	
}

void UGINICoreButton::SetEnhancedTriggeringInputAction_Implementation(UInputAction* NewTriggeringInputAction)
{
	SetTriggeringEnhancedInputAction(NewTriggeringInputAction);
}

void UGINICoreButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	Execute_SetText(this, ButtonCopyText);
}

void UGINICoreButton::NativeOnCurrentTextStyleChanged()
{
	UCommonTextStyle* CurrentStyle = GetCurrentTextStyle();
	if (CurrentStyle && ButtonText)
	{
		ButtonText->SetStyle(CurrentStyle->GetClass());
	}
}
