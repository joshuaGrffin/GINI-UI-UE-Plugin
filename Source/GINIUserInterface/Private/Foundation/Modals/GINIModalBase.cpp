// The Griffin Initiative


#include "Foundation/Modals/GINIModalBase.h"

#include "CommonTextBlock.h"

void UGINIModalBase::SetBodyText(const FText& InBodyText)
{
	ModalBodyText = InBodyText;
	if (ModalBody)
	{
		ModalBody->SetText(ModalBodyText);
	}
}

void UGINIModalBase::SetTitleText(const FText& InTitleText)
{
	ModalTitleText = InTitleText;
	if (ModalTitle)
	{
		ModalTitle->SetText(ModalTitleText);
	}
}

bool UGINIModalBase::NativeOnHandleBackAction()
{
	OnModalSelection.Broadcast(OnClosedDefaultSelection);
	return Super::NativeOnHandleBackAction();
}

void UGINIModalBase::NativeConstruct()
{
	Super::NativeConstruct();
	SetBodyText(ModalBodyText);
	SetTitleText(ModalTitleText);
}

UGINIModalBase::UGINIModalBase(const FObjectInitializer& InObjectInitializer)
	:Super(InObjectInitializer)
{
	bIsBackHandler = true;
	bIsBackActionDisplayedInActionBar = true;
	bIsModal = true;
	bSupportsActivationFocus = true;
	bAutoRestoreFocus = false;
}

#if WITH_EDITOR
void UGINIModalBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (!this)
	{
		return;
	}
	if (ModalBody)
	{
		ModalBody->SetText(ModalBodyText);
	}
	if (ModalTitle)
	{
		ModalTitle->SetText(ModalTitleText);
	}
}
#endif
