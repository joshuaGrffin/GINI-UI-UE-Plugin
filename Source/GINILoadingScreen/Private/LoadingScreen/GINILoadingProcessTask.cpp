// The Griffin Initiative


#include "LoadingScreen/GINILoadingProcessTask.h"

#include "LoadingScreen/GINILoadingScreenSubsystem.h"


UGINILoadingProcessTask* UGINILoadingProcessTask::CreateLoadingProcessTask(UObject* WorldContextObject,
                                                                           const FString& ShowLoadingScreenReason)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UGameInstance* GameInstance = World->GetGameInstance();
	UGINILoadingScreenSubsystem* LoadingScreenSubsystem = GameInstance ? GameInstance->GetSubsystem<UGINILoadingScreenSubsystem>() : nullptr;
	if (LoadingScreenSubsystem)
	{
		UGINILoadingProcessTask* NewLoadingTask = NewObject<UGINILoadingProcessTask>(LoadingScreenSubsystem);
		NewLoadingTask->SetShowLoadingScreenReason(ShowLoadingScreenReason);
		LoadingScreenSubsystem->RegisterLoadingProcessor(NewLoadingTask);

		return NewLoadingTask;
	}
	return nullptr;
}

void UGINILoadingProcessTask::Unregister()
{
	UGINILoadingScreenSubsystem* LoadingScreenSubsystem = Cast<UGINILoadingScreenSubsystem>(GetOuter());
	LoadingScreenSubsystem->UnregisterLoadingProcessor(this);
}

void UGINILoadingProcessTask::SetShowLoadingScreenReason(const FString& ShowLoadingScreenReason)
{
	Reason = ShowLoadingScreenReason;
}

bool UGINILoadingProcessTask::ShouldShowLoadingScreenReason_Implementation(FString& OutReason)
{
	OutReason = Reason;
	return true;
}
