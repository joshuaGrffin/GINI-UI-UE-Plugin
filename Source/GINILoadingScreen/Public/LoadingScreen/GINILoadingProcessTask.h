// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GINILoadingProcessInterface.h"
#include "UObject/Object.h"
#include "GINILoadingProcessTask.generated.h"

/**
 * 
 */
UCLASS()
class GINILOADINGSCREEN_API UGINILoadingProcessTask : public UObject, public IGINILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UGINILoadingProcessTask* CreateLoadingProcessTask(UObject* WorldContextObject, const FString& ShowLoadingScreenReason);

	UGINILoadingProcessTask()=default;
	UFUNCTION(BlueprintCallable)
	void Unregister();

	UFUNCTION(BlueprintCallable)
	void SetShowLoadingScreenReason(const FString& ShowLoadingScreenReason);

	virtual bool ShouldShowLoadingScreenReason_Implementation(FString& OutReason) override;
	
	UPROPERTY()
	FString Reason = FString();
};
