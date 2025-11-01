// The Griffin Initiative


#include "Foundation/Modals/GININotifyModal.h"

#include "Foundation/Buttons/GINICoreButton.h"

void UGININotifyModal::OnConfirmation_Implementation()
{
	OnModalSelection.Broadcast(0);
}

void UGININotifyModal::NativeConstruct()
{
	Super::NativeConstruct();
	if (!IsDesignTime())
	{
		ConfirmationButton->OnClicked().AddUObject(this, &UGININotifyModal::OnConfirmation);
	}
}

void UGININotifyModal::NativeDestruct()
{
	if (!IsDesignTime())
	{
		ConfirmationButton->OnClicked().RemoveAll(this);
	}
	Super::NativeDestruct();
}

UWidget* UGININotifyModal::NativeGetDesiredFocusTarget() const
{
	return ConfirmationButton;
}
