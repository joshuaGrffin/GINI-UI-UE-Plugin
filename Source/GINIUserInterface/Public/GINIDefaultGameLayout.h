// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "GINIDefaultGameLayout.generated.h"

struct FStreamableHandle;
class UCommonActivatableWidgetStack;
class UCommonActivatableWidgetQueue;
enum class EAsyncWidgetLayerState : uint8
{
	Canceled,
	Initialized,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetStackEmpty);

UCLASS(Abstract, meta = (DisableNativeTick))
class GINIUSERINTERFACE_API UGINIDefaultGameLayout : public UCommonUserWidget
{

	GENERATED_BODY()

public:
	UGINIDefaultGameLayout(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintPure, meta = (WorldContextObject="WorldContextObject"))
	static UGINIDefaultGameLayout* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure)
	static UGINIDefaultGameLayout* GetPrimaryGameLayoutForPlayerController(const APlayerController* PlayerController);
	UFUNCTION(BlueprintPure)
	static UGINIDefaultGameLayout* GetPrimaryGameLayoutForLocalPlayer(const ULocalPlayer* LocalPlayer);
	
	//Templated functions begin here:
	
	template <typename TActivatableWidget = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass)
	{
		return PushWidgetToLayerStackAsync<TActivatableWidget>(LayerName, bSuspendInputUntilComplete, ActivatableWidgetClass, [](EAsyncWidgetLayerState, TActivatableWidget*) {});
	}

	template <typename TActivatableWidget = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass, TFunction<void(EAsyncWidgetLayerState, TActivatableWidget*)> StateFunc)
	{
		static_assert(TIsDerivedFrom<TActivatableWidget, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");
		//#ToDo: Add support to suspending input for the owning player here:

		//static FName NAME_PushingWidgetToLayer("PushingWidgetToLayer");
		//const FName SuspendInputToken = bSuspendInputUntilComplete ? UCommonUIExtensions::SuspendInputForPlayer(GetOwningPlayer(), NAME_PushingWidgetToLayer) : NAME_None;

		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(ActivatableWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this,
			[this, LayerName, ActivatableWidgetClass, StateFunc]()
			{
				//UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);

				TActivatableWidget* Widget = PushWidgetToLayerStack<TActivatableWidget>(LayerName, ActivatableWidgetClass.Get(), [StateFunc](TActivatableWidget& WidgetToInit) {
					StateFunc(EAsyncWidgetLayerState::Initialized, &WidgetToInit);
				});

				StateFunc(EAsyncWidgetLayerState::AfterPush, Widget);
			})
		);

		// Setup a cancel delegate so that we can resume input if this handler is canceled.
		StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
			[this, StateFunc]()
			{
				//#ToDo: Add support to suspending input for the owning player here:
				//UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
				StateFunc(EAsyncWidgetLayerState::Canceled, nullptr);
			})
		);

		return StreamingHandle;
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&) {});
	}

	template <typename TActivatableWidget = UCommonActivatableWidget>
	TActivatableWidget* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass, TFunctionRef<void(TActivatableWidget&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<TActivatableWidget, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");

		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<TActivatableWidget>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}
	

	UFUNCTION(BlueprintCallable, Category = UI)
	void SetIsDormant(bool bIsDormant);
	UFUNCTION(BlueprintPure)
	bool IsDormant() const {return m_bIsDormant;};

	void RemoveWidgetFromLayer(UCommonActivatableWidget* InWidget, const FGameplayTag& LayerName);
	
	void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* InWidget);

	UFUNCTION()
	void OnDisplayedMenuWidgetChanged(UCommonActivatableWidget* Widget);
	UFUNCTION()
	void OnDisplayedMenuModalWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget);
	UFUNCTION()
	void OnDisplayedGameHudWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget);
	UFUNCTION()
	void OnDisplayedGameModalWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget);
	UFUNCTION()
	void OnDisplayedSystemWidgetChanged(UCommonActivatableWidget* CommonActivatableWidget);
	
	UCommonActivatableWidgetContainerBase* GetLayerWidget(const FGameplayTag& LayerTag) const;

	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnWidgetStackEmpty OnMenuStackEmpty;
	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnWidgetStackEmpty OnGameHudStackEmpty;
	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnWidgetStackEmpty OnMenuModalStackEmpty;
	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnWidgetStackEmpty OnGameModalStackEmpty;
	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnWidgetStackEmpty OnSystemQueueEmpty;
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION(BlueprintCallable, Category = "Layer")
	void RegisterLayer(UPARAM(meta=(Categories="UI.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);
	virtual void OnIsDormantChanged();
	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* InWidget, bool bIsTransitioning);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RegisterDefaultLayers();

	UFUNCTION(BlueprintPure)
	bool HasAnyWidgetsOnAnyLayer();
	
	UPROPERTY(BlueprintReadWrite, Category = UI, meta = (BindWidgetOptional))
	TObjectPtr<class UOverlay> BaseOverlay = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = UI)
	bool bRegisterDefaultLayers = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = UI, meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = UI, meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetStack> GameHudStack = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = UI, meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetQueue> MenuModalQueue = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = UI, meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetQueue> GameHudModalQueue = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = UI, meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetQueue> SystemQueue = nullptr;
	
private:
	bool m_bIsDormant = false;
	TArray<FName> m_SuspendInputTokens = {};
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> m_UILayers = {};
};
