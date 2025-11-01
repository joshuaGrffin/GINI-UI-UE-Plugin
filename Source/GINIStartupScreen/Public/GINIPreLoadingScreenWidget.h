// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Accessibility/SlateAccessibleWidgets.h"
#include "Widgets/SCompoundWidget.h"
class FReferenceCollector;

/**
 * 
 */
class GINISTARTUPSCREEN_API SGINIPreLoadingScreenWidget : public SCompoundWidget, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SGINIPreLoadingScreenWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
};
