// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GINIBaseModalInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGINIBaseModalInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GINIUSERINTERFACE_API IGINIBaseModalInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
	void SetTitle(const FText& Title);
	virtual void SetTitle_Implementation(const FText& Title) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
	void SetBody(const FText& Body);
	virtual void SetBody_Implementation(const FText& Body) = 0;
};
