// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateStyles.h"
#include "Fonts/SlateFontInfo.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Input/Reply.h"
#include "Layout/Margin.h"
#include "Misc/Attribute.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SBorder.h"
#include "Framework/SlateDelegates.h"

class IErrorReportingWidget;
class SBox;
class SHorizontalBox;
enum class ETextFlowDirection : uint8;
enum class ETextShapingMethod : uint8;

/**
 * Editable text box widget
 */
class GINIUSERINTERFACE_API SGINIEditableTextBox : public SBorder
{
/*
public:

	SLATE_BEGIN_ARGS( SGINIEditableTextBox )
		: _Style()
		, _Text()
		, _HintText()
		, _SearchText()
		, _Font()
		, _ForegroundColor()
		, _ReadOnlyForegroundColor()
		, _FocusedForegroundColor()
		, _IsReadOnly( false )
		, _IsPassword( false )
		, _IsCaretMovedWhenGainFocus ( true )
		, _SelectAllTextWhenFocused( false )
		, _RevertTextOnEscape( false )
		, _ClearKeyboardFocusOnCommit( true )
		, _Justification(ETextJustify::Left)
		, _MinimumLength(-1)
		, _MaximumLength(-1)
		, _AllowContextMenu(true)
		, _MinDesiredWidth( 0.0f )
		, _SelectAllTextOnCommit( false )
		, _SelectWordOnMouseDoubleClick(true)
		, _BackgroundColor()
		, _Padding()
		, _ErrorReporting()
		, _VirtualKeyboardOptions(FVirtualKeyboardOptions())
		, _VirtualKeyboardTrigger(EVirtualKeyboardTrigger::OnFocusByPointer)
		, _VirtualKeyboardDismissAction(EVirtualKeyboardDismissAction::TextChangeOnDismiss)
		, _OverflowPolicy()
		{
		}


		SLATE_STYLE_ARGUMENT( FGINIEditableTextBoxStyle, Style )

		SLATE_ATTRIBUTE( FText, Text )

		SLATE_ATTRIBUTE( FText, HintText )

		SLATE_ATTRIBUTE( FText, SearchText )
		
		SLATE_ATTRIBUTE( int32, MinimumLength )
		
		SLATE_ATTRIBUTE( FSlateFontInfo, Font )

		SLATE_ATTRIBUTE( FSlateColor, ForegroundColor )
		
		SLATE_ATTRIBUTE( FSlateColor, ReadOnlyForegroundColor )

		SLATE_ATTRIBUTE(FSlateColor, FocusedForegroundColor)

		SLATE_ATTRIBUTE( bool, IsReadOnly )

		SLATE_ATTRIBUTE( bool, IsPassword )

		SLATE_ATTRIBUTE( bool, IsCaretMovedWhenGainFocus )

		SLATE_ATTRIBUTE( bool, SelectAllTextWhenFocused )

		SLATE_ATTRIBUTE( bool, RevertTextOnEscape )

		SLATE_ATTRIBUTE( bool, ClearKeyboardFocusOnCommit )

		SLATE_ATTRIBUTE(ETextJustify::Type, Justification)

		SLATE_ATTRIBUTE(int32, MaximumLength)

		SLATE_ATTRIBUTE(bool, AllowContextMenu)

		SLATE_EVENT(FOnContextMenuOpening, OnContextMenuOpening)

		SLATE_EVENT(FMenuExtensionDelegate, ContextMenuExtender)

		SLATE_EVENT( FOnTextChanged, OnTextChanged )

		SLATE_EVENT( FOnTextCommitted, OnTextCommitted )

		SLATE_EVENT( FOnVerifyTextChanged, OnVerifyTextChanged )

		SLATE_ATTRIBUTE( float, MinDesiredWidth )

		SLATE_ATTRIBUTE( bool, SelectAllTextOnCommit )

		SLATE_ATTRIBUTE(bool, SelectWordOnMouseDoubleClick)

		SLATE_EVENT(FOnKeyChar, OnKeyCharHandler)

		SLATE_EVENT(FOnKeyDown, OnKeyDownHandler)

		SLATE_ATTRIBUTE( FSlateColor, BackgroundColor )

		SLATE_ATTRIBUTE( FMargin, Padding )

		SLATE_ARGUMENT( TSharedPtr<class IErrorReportingWidget>, ErrorReporting )

		SLATE_ATTRIBUTE(EKeyboardType, VirtualKeyboardType)

		SLATE_ARGUMENT(FVirtualKeyboardOptions, VirtualKeyboardOptions)

		SLATE_ATTRIBUTE(EVirtualKeyboardTrigger, VirtualKeyboardTrigger)

		SLATE_ATTRIBUTE(EVirtualKeyboardDismissAction, VirtualKeyboardDismissAction)

		SLATE_ARGUMENT(TOptional<ETextShapingMethod>, TextShapingMethod)

		SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

		SLATE_ARGUMENT(TOptional<ETextOverflowPolicy>, OverflowPolicy)
	SLATE_END_ARGS()

	SGINIEditableTextBox();
	virtual ~SGINIEditableTextBox();
	

	void Construct( const FArguments& InArgs );

	FText GetText() const
	{
		return EditableText->GetText();
	}

	void SetStyle(const FGINIEditableTextBoxStyle* InStyle);

	void SetTextBlockStyle(const FTextBlockStyle* InTextStyle);

	void SetText( const TAttribute< FText >& InNewText );
	
	FText GetHintText() const
	{
		return EditableText->GetHintText();
	}

	void SetHintText( const TAttribute< FText >& InHintText );
	
	void SetSearchText(const TAttribute<FText>& InSearchText);

	FText GetSearchText() const;

	void SetIsReadOnly( TAttribute< bool > InIsReadOnly );

	bool IsReadOnly() const { return EditableText->IsTextReadOnly(); }
	
	void SetIsPassword( TAttribute< bool > InIsPassword );

	bool IsPassword() const { return EditableText->IsTextPassword(); }

	void SetAllowContextMenu(TAttribute< bool > InAllowContextMenu);

	void SetVirtualKeyboardDismissAction(TAttribute< EVirtualKeyboardDismissAction > InVirtualKeyboardDismissAction);
	
	void SetFont(const TAttribute<FSlateFontInfo>& InFont);

	void SetTextBoxForegroundColor(const TAttribute<FSlateColor>& InForegroundColor);

	void SetTextBoxBackgroundColor(const TAttribute<FSlateColor>& InBackgroundColor);

	void SetReadOnlyForegroundColor(const TAttribute<FSlateColor>& InReadOnlyForegroundColor);

	void SetFocusedForegroundColor(const TAttribute<FSlateColor>& InFocusedForegroundColor);

	void SetMaximumLength(const TAttribute<int32>& InMaximumLength);

	void SetMinimumLength(const TAttribute<int32>& InMinimumLength);
	
	void SetMinimumDesiredWidth(const TAttribute<float>& InMinimumDesiredWidth);

	void SetIsCaretMovedWhenGainFocus(const TAttribute<bool>& InIsCaretMovedWhenGainFocus);

	void SetSelectAllTextWhenFocused(const TAttribute<bool>& InSelectAllTextWhenFocused);

	void SetRevertTextOnEscape(const TAttribute<bool>& InRevertTextOnEscape);

	void SetClearKeyboardFocusOnCommit(const TAttribute<bool>& InClearKeyboardFocusOnCommit);

	void SetSelectAllTextOnCommit(const TAttribute<bool>& InSelectAllTextOnCommit);

	void SetSelectWordOnMouseDoubleClick(const TAttribute<bool>& InSelectWordOnMouseDoubleClick);

	void SetJustification(const TAttribute<ETextJustify::Type>& InJustification);

	void SetError( const FText& InError );
	void SetError( const FString& InError );

	void SetOnKeyCharHandler(FOnKeyChar InOnKeyCharHandler);

	void SetOnKeyDownHandler(FOnKeyDown InOnKeyDownHandler);

	void SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod);

	void SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection);

	void SetOverflowPolicy(TOptional<ETextOverflowPolicy> InOverflowPolicy);

	void SetHoveredSound(TOptional<FSlateSound> InHoveredSound);

	void SetClickedSound(TOptional<FSlateSound> InClickedSound);
	
	void SetOnHovered(FSimpleDelegate InOnHovered);

	void SetOnUnhovered(FSimpleDelegate InOnUnhovered);
	
	bool AnyTextSelected() const;

	void SelectAllText();

	void ClearSelection();

	FText GetSelectedText() const;

	void GoTo(const FTextLocation& NewLocation);

	void GoTo(const ETextLocation NewLocation)
	{
		EditableText->GoTo(NewLocation);
	}

	void ScrollTo(const FTextLocation& NewLocation);

	void ScrollTo(const ETextLocation NewLocation)
	{
		EditableText->GoTo(NewLocation);
	}

	void BeginSearch(const FText& InSearchText, const ESearchCase::Type InSearchCase = ESearchCase::IgnoreCase, const bool InReverse = false);

	void AdvanceSearch(const bool InReverse = false);

	void EnableTextInputMethodContext();

	bool HasError() const;

	// SWidget overrides
	virtual bool SupportsKeyboardFocus() const override;
	virtual bool HasKeyboardFocus() const override;
	virtual FReply OnFocusReceived( const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent ) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

protected:
#if WITH_ACCESSIBILITY
	virtual TSharedRef<FSlateAccessibleWidget> CreateAccessibleWidget() override;
	virtual TOptional<FText> GetDefaultAccessibleText(EAccessibleType AccessibleType = EAccessibleType::Main) const override;
#endif
	void OnEditableTextChanged(const FText& InText);

	void OnEditableTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	void PlayHoverSound() const;
	
	void ExecuteHoverStateChanged(bool bPlaySound);
	
	const FGINIEditableTextBoxStyle* Style;


	TAttribute<int32> MinimumLength;

	TSharedPtr< SBox > PaddingBox;

	TSharedPtr< SEditableText > EditableText;

	TAttribute<FMargin> PaddingOverride;

	TAttribute<FSlateFontInfo> FontOverride;

	TAttribute<FSlateColor> ForegroundColorOverride;

	TAttribute<FSlateColor> BackgroundColorOverride;

	TAttribute<FSlateColor> ReadOnlyForegroundColorOverride;

	TAttribute<FSlateColor> FocusedForegroundColorOverride;

	TAttribute<int32> MaximumLength;

	TSharedPtr<SHorizontalBox> Box;

	TSharedPtr<class IErrorReportingWidget> ErrorReporting;

	FOnTextChanged OnTextChanged;

	FOnTextCommitted OnTextCommitted;	

	FOnVerifyTextChanged OnVerifyTextChanged;

	FSimpleDelegate OnHovered;

	FSimpleDelegate OnUnhovered;

	FSlateSound HoveredSound;
	FSlateSound ClickedSound;

private:

	bool VerifyLength(const FText& InText, FText& OutErrorMessage);
	
	FMargin DeterminePadding() const;
	FSlateFontInfo DetermineFont() const;
	FSlateColor DetermineBackgroundColor() const;
	FSlateColor DetermineForegroundColor() const;

	const FSlateBrush* BorderImageNormal;
	const FSlateBrush* BorderImageHovered;
	const FSlateBrush* BorderImageFocused;
	const FSlateBrush* BorderImageReadOnly;

	const FSlateBrush* GetBorderImage() const;
*/
};
