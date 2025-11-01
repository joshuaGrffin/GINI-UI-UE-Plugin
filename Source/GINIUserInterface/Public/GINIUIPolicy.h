// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "GINIDefaultGameLayout.h"
#include "UObject/Object.h"
#include "GINIUIPolicy.generated.h"

class UGINIUIManagerSubsystem;
class UGINIDefaultGameLayout;
class ULocalPlayer;

UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
	PrimaryPlayerOnly,
	SingleToggle,
	Simultaneous
};

USTRUCT()
struct FRootLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;
	UPROPERTY()
	TObjectPtr<UGINIDefaultGameLayout> GameLayout = nullptr;
	UPROPERTY()
	bool bAddedToViewport = false;

	FRootLayoutInfo() = default;
	FRootLayoutInfo(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout, bool bIsInViewport);
	
	bool operator==(const ULocalPlayer* Player) const {return LocalPlayer == Player;};
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable, Within = GINIUIManagerSubsystem)
class GINIUSERINTERFACE_API UGINIUIPolicy : public UObject
{
	GENERATED_BODY()
public:
	template <typename UIPolicyClass = UGINIUIPolicy>
	static UIPolicyClass* GetUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<UIPolicyClass>(GetUIPolicy(WorldContextObject));
	}

	static UGINIUIPolicy* GetUIPolicy(const UObject* WorldContextObject);
	virtual UWorld* GetWorld() const override;
	UGINIUIManagerSubsystem* GetUIManagerSubsystem() const;
	UGINIDefaultGameLayout* GetRootLayout(const ULocalPlayer* LocalPlayer) const;
	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const;
	void RequestControl(UGINIDefaultGameLayout* GameLayout);
protected:
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout);
	virtual void OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout);
	virtual void OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout);
	virtual void OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UGINIDefaultGameLayout* GameLayout);
	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	TSubclassOf<UGINIDefaultGameLayout> GetLayoutWidgetClass(ULocalPlayer* LocalPlayer) const;
private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryPlayerOnly;

	void OnLocalPlayerAdded(ULocalPlayer* LocalPlayer);
	void OnLocalPlayerRemoved(ULocalPlayer* LocalPlayer);
	void OnLocalPlayerDestroyed(ULocalPlayer* LocalPlayer);
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGINIDefaultGameLayout> LayoutClass = nullptr;
	UPROPERTY(Transient)
	TArray<FRootLayoutInfo> RootViewportLayouts = {};

	friend class UGINIUIManagerSubsystem;
	
};
