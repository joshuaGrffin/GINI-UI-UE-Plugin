// The Griffin Initiative


#include "Foundation/Modals/GINIConfirmDenyModal.h"

#include "Foundation/Buttons/GINICoreButton.h"

void UGINIConfirmDenyModal::OnDenial_Implementation()
{
	OnModalSelection.Broadcast(0);
}

void UGINIConfirmDenyModal::OnConfirmation_Implementation()
{
	OnModalSelection.Broadcast(1);
}

void UGINIConfirmDenyModal::NativeConstruct()
{
	Super::NativeConstruct();
	if (!IsDesignTime())
	{
		ConfirmButton->OnClicked().AddUObject(this, &ThisClass::OnConfirmation);
		DenyButton->OnClicked().AddUObject(this, &ThisClass::OnDenial);
	}
}

void UGINIConfirmDenyModal::NativeDestruct()
{
	if (!IsDesignTime())
	{
		if (ConfirmButton)
		{
			ConfirmButton->OnClicked().RemoveAll(this);
		}
		if (DenyButton)
		{
			DenyButton->OnClicked().RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

UWidget* UGINIConfirmDenyModal::NativeGetDesiredFocusTarget() const
{
	return ConfirmButton;
}
