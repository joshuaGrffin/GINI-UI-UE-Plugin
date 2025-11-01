// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"

#include "SlateStyles.generated.h"


USTRUCT(BlueprintType)
struct GINIUSERINTERFACE_API FGINIEditableTextBoxStyle : public FEditableTextBoxStyle
{
	GENERATED_BODY()
public:
	FGINIEditableTextBoxStyle() = default;
	~FGINIEditableTextBoxStyle() = default;

	virtual void GetResources( TArray< const FSlateBrush* >& OutBrushes ) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound, meta=(DisplayName="Hover Sound"))\
	FSlateSound HoverSound;
	FGINIEditableTextBoxStyle& SetHoverSound(const FSlateSound& InHoverSound) {HoverSound = InHoverSound; return *this;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound, meta=(DisplayName="Clicked Sound"))
	FSlateSound ClickedSound;
	FGINIEditableTextBoxStyle& SetClickSound(const FSlateSound& InClickSound){ClickedSound = InClickSound; return *this;}
	
};