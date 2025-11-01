// The Griffin Initiative


#include "Foundation/AsyncActions/PushModalToLayerAsync.h"

#include "GINIDefaultGameLayout.h"

void UPushModalToLayerAsync::Cancel()
{
	Super::Cancel();
}

UPushModalToLayerAsync* UPushModalToLayerAsync::PushSystemModal(APlayerController* OwningPlayer,
	TSoftClassPtr<UGINIModalBase> InWidgetClass, FGameplayTag LayerName, const FText& ModalTitle,
	const FText& ModalBody, bool bSuspendInputUntilComplete)
{
	if (InWidgetClass.IsNull())
	{
		FFrame::KismetExecutionMessage(TEXT("PushContentToLayerAsync was passed a null WidgetClass"), ELogVerbosity::Error);
		return nullptr;
	}
	if (UWorld* World = GEngine->GetWorldFromContextObject(OwningPlayer, EGetWorldErrorMode::LogAndReturnNull))
	{
		UPushModalToLayerAsync* Action = NewObject<UPushModalToLayerAsync>();
		Action->OwningPlayerPtr = OwningPlayer;
		Action->WidgetClass = InWidgetClass;
		Action->bSuspendInputUntilComplete = bSuspendInputUntilComplete;
		Action->LayerName = LayerName;
		Action->RegisterWithGameInstance(World);
		Action->TitleText = ModalTitle;
		Action->BodyText = ModalBody;
		return Action;
	}
	return nullptr;
}

void UPushModalToLayerAsync::Activate()
{
	if (UGINIDefaultGameLayout* Layout = UGINIDefaultGameLayout::GetPrimaryGameLayoutForPlayerController(OwningPlayerPtr.Get()))
	{
		TWeakObjectPtr<UPushModalToLayerAsync> WeakThis = MakeWeakObjectPtr(this);
		StreamingHandle = Layout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(LayerName, bSuspendInputUntilComplete, WidgetClass, [this, WeakThis](EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)
		{
			if (WeakThis.IsValid())
			{
				OnLoadUpdate(State, Widget);
				return;
			}
			
			SetReadyToDestroy();
		});
	}
	else
	{
		SetReadyToDestroy();
	}
}

void UPushModalToLayerAsync::OnLoadUpdate(EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)
{
	switch (State) {
	case EAsyncWidgetLayerState::Canceled:
		SetReadyToDestroy();
		break;
	case EAsyncWidgetLayerState::Initialized:
		break;
	case EAsyncWidgetLayerState::AfterPush:

		UGINIModalBase* NewModal = Cast<UGINIModalBase>(Widget);
		NewModal->SetTitleText(TitleText);
		NewModal->SetBodyText(BodyText);
		AfterPush.Broadcast(NewModal);
		SetReadyToDestroy();
		break;
	}
}
