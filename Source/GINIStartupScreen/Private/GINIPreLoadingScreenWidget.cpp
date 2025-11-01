// The Griffin Initiative


#include "GINIPreLoadingScreenWidget.h"


void SGINIPreLoadingScreenWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.Padding(0)
		.BorderBackgroundColor(FLinearColor::Green)
	];
}

void SGINIPreLoadingScreenWidget::AddReferencedObjects(FReferenceCollector& Collector)
{
	// DO NOTHING FOR NOW
}

FString SGINIPreLoadingScreenWidget::GetReferencerName() const
{
	return TEXT("GINIPreLoadingScreenWidget");
}
