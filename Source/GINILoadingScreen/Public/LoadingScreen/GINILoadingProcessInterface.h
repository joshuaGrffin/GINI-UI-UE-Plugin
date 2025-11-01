// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GINILoadingProcessInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGINILoadingProcessInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GINILOADINGSCREEN_API IGINILoadingProcessInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	static bool ShouldShowLoadingScreen(UObject* TestObject, FString& OutReason);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldShowLoadingScreenReson(FString& OutReason);
	virtual bool ShouldShowLoadingScreenReason_Implementation(FString& OutReason){return false;};
	
};
