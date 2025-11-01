// The Griffin Initiative


#include "Foundation/AsyncActions/PushContentToLayerAsync.h"

#include "GINIDefaultGameLayout.h"
#include "Engine/StreamableManager.h"

void UPushContentToLayerAsync::Cancel()
{
	Super::Cancel();
	if (StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}
}

UPushContentToLayerAsync* UPushContentToLayerAsync::PushContentToLayerAsync(APlayerController* InOwningPlayer,
	TSoftClassPtr<UCommonActivatableWidget> InWidgetClass, FGameplayTag InLayerName, bool bInSuspendInputUntilComplete)
{
	if (InWidgetClass.IsNull())
	{
		FFrame::KismetExecutionMessage(TEXT("PushContentToLayerAsync was passed a null WidgetClass"), ELogVerbosity::Error);
		return nullptr;
	}
	if (UWorld* World = GEngine->GetWorldFromContextObject(InOwningPlayer, EGetWorldErrorMode::LogAndReturnNull))
	{
		UPushContentToLayerAsync* Action = NewObject<UPushContentToLayerAsync>();
		Action->OwningPlayerPtr = InOwningPlayer;
		Action->WidgetClass = InWidgetClass;
		Action->bSuspendInputUntilComplete = bInSuspendInputUntilComplete;
		Action->LayerName = InLayerName;
		Action->RegisterWithGameInstance(World);
		return Action;
	}
	return nullptr;
}

void UPushContentToLayerAsync::Activate()
{
	if (UGINIDefaultGameLayout* Layout = UGINIDefaultGameLayout::GetPrimaryGameLayoutForPlayerController(OwningPlayerPtr.Get()))
	{
		TWeakObjectPtr<UPushContentToLayerAsync> WeakThis = MakeWeakObjectPtr(this);
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

void UPushContentToLayerAsync::OnLoadUpdate(EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)
{
	switch (State) {
	case EAsyncWidgetLayerState::Canceled:
		SetReadyToDestroy();
		break;
	case EAsyncWidgetLayerState::Initialized:
		BeforePush.Broadcast(Widget);
		break;
	case EAsyncWidgetLayerState::AfterPush:
		AfterPush.Broadcast(Widget);
		SetReadyToDestroy();
		break;
	}
}
