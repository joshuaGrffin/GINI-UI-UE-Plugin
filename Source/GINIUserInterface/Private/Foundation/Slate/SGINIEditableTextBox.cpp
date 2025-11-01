// The Griffin Initiative


#include "Foundation/Slate/SGINIEditableTextBox.h"


#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Notifications/SPopUpErrorText.h"

#if WITH_ACCESSIBILITY
#include "Widgets/Accessibility/SlateAccessibleWidgets.h"
#endif

#define LOCTEXT_NAMESPACE "GINITextHelper"
/*
SGINIEditableTextBox::SGINIEditableTextBox()
{
#if WITH_ACCESSIBILITY
	AccessibleBehavior = EAccessibleBehavior::Auto;
	bCanChildrenBeAccessible = false;
#endif
}

SGINIEditableTextBox::~SGINIEditableTextBox() = default;

void SGINIEditableTextBox::Construct( const FArguments& InArgs )
{
	check (InArgs._Style);
	SetStyle(InArgs._Style);

	PaddingOverride = InArgs._Padding;
	FontOverride = InArgs._Font;
	ForegroundColorOverride = InArgs._ForegroundColor;
	BackgroundColorOverride = InArgs._BackgroundColor;
	ReadOnlyForegroundColorOverride = InArgs._ReadOnlyForegroundColor;
	FocusedForegroundColorOverride = InArgs._FocusedForegroundColor;
	MaximumLength = InArgs._MaximumLength;
	OnTextChanged = InArgs._OnTextChanged;
	OnVerifyTextChanged = InArgs._OnVerifyTextChanged;
	OnTextCommitted = InArgs._OnTextCommitted;

	SBorder::Construct( SBorder::FArguments()
		.BorderImage( this, &SGINIEditableTextBox::GetBorderImage )
		.BorderBackgroundColor( this, &SGINIEditableTextBox::DetermineBackgroundColor )
		.ForegroundColor( this, &SGINIEditableTextBox::DetermineForegroundColor )
		.Padding(0.f)
		[
			SAssignNew( Box, SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.FillWidth(1)
			[
				SAssignNew(PaddingBox, SBox)
				.Padding( this, &SGINIEditableTextBox::DeterminePadding )
				.VAlign(VAlign_Center)
				[
					SAssignNew( EditableText, SEditableText )
					.Text( InArgs._Text )
					.HintText( InArgs._HintText )
					.SearchText( InArgs._SearchText )
					.Font( this, &SGINIEditableTextBox::DetermineFont )
					.IsReadOnly( InArgs._IsReadOnly )
					.IsPassword( InArgs._IsPassword )
					.IsCaretMovedWhenGainFocus( InArgs._IsCaretMovedWhenGainFocus )
					.SelectAllTextWhenFocused( InArgs._SelectAllTextWhenFocused )
					.RevertTextOnEscape( InArgs._RevertTextOnEscape )
					.ClearKeyboardFocusOnCommit( InArgs._ClearKeyboardFocusOnCommit )
					.Justification( InArgs._Justification )
					.AllowContextMenu( InArgs._AllowContextMenu )
					.OnContextMenuOpening( InArgs._OnContextMenuOpening )
					.ContextMenuExtender( InArgs._ContextMenuExtender )
					.OnTextChanged(this, &SGINIEditableTextBox::OnEditableTextChanged)
					.OnTextCommitted(this, &SGINIEditableTextBox::OnEditableTextCommitted)
					.MinDesiredWidth( InArgs._MinDesiredWidth )
					.SelectAllTextOnCommit( InArgs._SelectAllTextOnCommit )
					.SelectWordOnMouseDoubleClick(InArgs._SelectWordOnMouseDoubleClick)
					.OnKeyCharHandler( InArgs._OnKeyCharHandler )			
					.OnKeyDownHandler( InArgs._OnKeyDownHandler )
					.VirtualKeyboardType( InArgs._VirtualKeyboardType )
					.VirtualKeyboardOptions( InArgs._VirtualKeyboardOptions )
					.VirtualKeyboardTrigger( InArgs._VirtualKeyboardTrigger )
					.VirtualKeyboardDismissAction( InArgs._VirtualKeyboardDismissAction )
					.TextShapingMethod(InArgs._TextShapingMethod)
					.TextFlowDirection( InArgs._TextFlowDirection )
					.OverflowPolicy(InArgs._OverflowPolicy)
				]
			]
		]
	);

	ErrorReporting = InArgs._ErrorReporting;
	if ( ErrorReporting.IsValid() )
	{
		Box->AddSlot()
		.AutoWidth()
		.Padding(3,0)
		[
			ErrorReporting->AsWidget()
		];
	}
	else
	{
		// this also creates a default widget
		// if we don't create the widget in Construct() 
		// it will get created in OnEditableTextChanged()
		// create it now so that the default size of the textbox
		// won't grow after user use it once
		SetError(FText::GetEmpty());
	}
}

void SGINIEditableTextBox::SetStyle(const FGINIEditableTextBoxStyle* InStyle)
{
	Style = InStyle;

	if ( Style == nullptr )
	{
		FArguments Defaults;
		Style = Defaults._Style;
	}

	check(Style);

	BorderImageNormal = &Style->BackgroundImageNormal;
	BorderImageHovered = &Style->BackgroundImageHovered;
	BorderImageFocused = &Style->BackgroundImageFocused;
	BorderImageReadOnly = &Style->BackgroundImageReadOnly;

	SetTextBlockStyle(&Style->TextStyle);
}

void SGINIEditableTextBox::SetTextBlockStyle(const FTextBlockStyle* InTextStyle)
{
	// The Construct() function will call this before EditableText exists,
	// so we need a guard here to ignore that function call.
	if (EditableText.IsValid())
	{
		EditableText->SetTextBlockStyle(InTextStyle);
	}
}

void SGINIEditableTextBox::SetText( const TAttribute< FText >& InNewText )
{
	EditableText->SetText( InNewText );
}


void SGINIEditableTextBox::SetError( const FText& InError )
{
	SetError( InError.ToString() );
}


void SGINIEditableTextBox::SetError( const FString& InError )
{
	const bool bHaveError = !InError.IsEmpty();

	if ( !ErrorReporting.IsValid() )
	{
		// No error reporting was specified; make a default one
		TSharedPtr<SPopupErrorText> ErrorTextWidget;
		Box->AddSlot()
		.AutoWidth()
		.Padding(3,0)
		[
			SAssignNew( ErrorTextWidget, SPopupErrorText )
		];
		ErrorReporting = ErrorTextWidget;
	}

	ErrorReporting->SetError( InError );
}


void SGINIEditableTextBox::SetOnKeyCharHandler(FOnKeyChar InOnKeyCharHandler)
{
	EditableText->SetOnKeyCharHandler(InOnKeyCharHandler);
}


void SGINIEditableTextBox::SetOnKeyDownHandler(FOnKeyDown InOnKeyDownHandler)
{
	EditableText->SetOnKeyDownHandler(InOnKeyDownHandler);
}


void SGINIEditableTextBox::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	EditableText->SetTextShapingMethod(InTextShapingMethod);
}


void SGINIEditableTextBox::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	EditableText->SetTextFlowDirection(InTextFlowDirection);
}


void SGINIEditableTextBox::SetOverflowPolicy(TOptional<ETextOverflowPolicy> InOverflowPolicy)
{
	EditableText->SetOverflowPolicy(InOverflowPolicy);
}

void SGINIEditableTextBox::SetHoveredSound(TOptional<FSlateSound> InHoveredSound)
{
	HoveredSound = InHoveredSound.Get(Style->HoverSound);
}

void SGINIEditableTextBox::SetClickedSound(TOptional<FSlateSound> InClickedSound)
{
	ClickedSound = InClickedSound.Get(Style->ClickedSound);
}

void SGINIEditableTextBox::SetOnHovered(FSimpleDelegate InOnHovered)
{
	OnHovered = InOnHovered;
}

void SGINIEditableTextBox::SetOnUnhovered(FSimpleDelegate InOnUnhovered)
{
	OnUnhovered = InOnUnhovered;
}

bool SGINIEditableTextBox::AnyTextSelected() const
{
	return EditableText->AnyTextSelected();
}


void SGINIEditableTextBox::SelectAllText()
{
	EditableText->SelectAllText();
}


void SGINIEditableTextBox::ClearSelection()
{
	EditableText->ClearSelection();
}


FText SGINIEditableTextBox::GetSelectedText() const
{
	return EditableText->GetSelectedText();
}

void SGINIEditableTextBox::GoTo(const FTextLocation& NewLocation)
{
	EditableText->GoTo(NewLocation);
}

void SGINIEditableTextBox::ScrollTo(const FTextLocation& NewLocation)
{
	EditableText->ScrollTo(NewLocation);
}

void SGINIEditableTextBox::BeginSearch(const FText& InSearchText, const ESearchCase::Type InSearchCase, const bool InReverse)
{
	EditableText->BeginSearch(InSearchText, InSearchCase, InReverse);
}

void SGINIEditableTextBox::AdvanceSearch(const bool InReverse)
{
	EditableText->AdvanceSearch(InReverse);
}

bool SGINIEditableTextBox::HasError() const
{
	return ErrorReporting.IsValid() && ErrorReporting->HasError();
}

const FSlateBrush* SGINIEditableTextBox::GetBorderImage() const
{
	if ( EditableText->IsTextReadOnly() )
	{
		return BorderImageReadOnly;
	}
	else if ( EditableText->HasKeyboardFocus() )
	{
		return BorderImageFocused;
	}
	else
	{
		if ( EditableText->IsHovered() )
		{
			return BorderImageHovered;
		}
		else
		{
			return BorderImageNormal;
		}
	}
}


bool SGINIEditableTextBox::SupportsKeyboardFocus() const
{
	return StaticCastSharedPtr<SWidget>(EditableText)->SupportsKeyboardFocus();
}


bool SGINIEditableTextBox::HasKeyboardFocus() const
{
	// Since keyboard focus is forwarded to our editable text, we will test it instead
	return SBorder::HasKeyboardFocus() || EditableText->HasKeyboardFocus();
}


FReply SGINIEditableTextBox::OnFocusReceived( const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent )
{
	FReply Reply = FReply::Handled();

	if ( InFocusEvent.GetCause() != EFocusCause::Cleared )
	{
		// Forward keyboard focus to our editable text widget
		Reply.SetUserFocus(EditableText.ToSharedRef(), InFocusEvent.GetCause());
	}
	

	return Reply;
}


FReply SGINIEditableTextBox::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Escape && EditableText->HasKeyboardFocus())
	{
		// Clear focus
		return FReply::Handled().SetUserFocus(SharedThis(this), EFocusCause::Cleared);
	}
	
	return FReply::Unhandled();
}

FReply SGINIEditableTextBox::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		
	}
	
	return SBorder::OnKeyUp(MyGeometry, InKeyEvent);
}

void SGINIEditableTextBox::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();
	SBorder::OnMouseEnter(MyGeometry, MouseEvent);
	if ( !bWasHovered && IsHovered() )
	{
		ExecuteHoverStateChanged(true);
	}
}

void SGINIEditableTextBox::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();
	SBorder::OnMouseLeave(MouseEvent);
	if ( bWasHovered && !IsHovered() )
	{
		ExecuteHoverStateChanged(true);
	}
}

bool SGINIEditableTextBox::VerifyLength(const FText& InText, FText& OutErrorMessage)
{
	const int TextLength = InText.ToString().Len();
	if (MaximumLength.Get() > 0 && TextLength > MaximumLength.Get())
	{
		OutErrorMessage = FText::Format(LOCTEXT("TextTooLong", "The Entry is too long it uses {0} characters of {1} allowed."), TextLength, MaximumLength.Get());
		return false;
	}

	if (MinimumLength.Get() > 0 && TextLength < MinimumLength.Get())
	{
		OutErrorMessage = FText::Format(LOCTEXT("TextTooShort", "The Entry is too short. It has {0} Characters with a minimum of {1}"), TextLength, MinimumLength.Get());
		return false;
	}

	return true;
}

FMargin SGINIEditableTextBox::DeterminePadding() const
{
	check(Style);
	return PaddingOverride.IsSet() ? PaddingOverride.Get() : Style->Padding;
}

FSlateFontInfo SGINIEditableTextBox::DetermineFont() const
{
	check(Style);
	return FontOverride.IsSet() ? FontOverride.Get() : Style->TextStyle.Font;
}

FSlateColor SGINIEditableTextBox::DetermineBackgroundColor() const
{
	check(Style);
	return BackgroundColorOverride.IsSet() ? BackgroundColorOverride.Get() : Style->BackgroundColor;
}

FSlateColor SGINIEditableTextBox::DetermineForegroundColor() const
{
	check(Style);  
	
	if (EditableText->IsTextReadOnly())
	{
		if (ReadOnlyForegroundColorOverride.IsSet())
		{
			return ReadOnlyForegroundColorOverride.Get();
		}
		if (ForegroundColorOverride.IsSet())
		{
			return ForegroundColorOverride.Get();
		}

		return Style->ReadOnlyForegroundColor;
	}
	else if(HasKeyboardFocus())
	{
		return FocusedForegroundColorOverride.IsSet() ? FocusedForegroundColorOverride.Get() : Style->FocusedForegroundColor;
	}
	else
	{
		return ForegroundColorOverride.IsSet() ? ForegroundColorOverride.Get() : Style->ForegroundColor;
	}
}

void SGINIEditableTextBox::SetHintText(const TAttribute< FText >& InHintText)
{
	EditableText->SetHintText(InHintText);
}


void SGINIEditableTextBox::SetSearchText(const TAttribute<FText>& InSearchText)
{
	EditableText->SetSearchText(InSearchText);
}


FText SGINIEditableTextBox::GetSearchText() const
{
	return EditableText->GetSearchText();
}


void SGINIEditableTextBox::SetIsReadOnly(TAttribute< bool > InIsReadOnly)
{
	EditableText->SetIsReadOnly(InIsReadOnly);
}


void SGINIEditableTextBox::SetIsPassword(TAttribute< bool > InIsPassword)
{
	EditableText->SetIsPassword(InIsPassword);
}


void SGINIEditableTextBox::SetFont(const TAttribute<FSlateFontInfo>& InFont)
{
	FontOverride = InFont;
}

void SGINIEditableTextBox::SetTextBoxForegroundColor(const TAttribute<FSlateColor>& InForegroundColor)
{
	ForegroundColorOverride = InForegroundColor;
}

void SGINIEditableTextBox::SetTextBoxBackgroundColor(const TAttribute<FSlateColor>& InBackgroundColor)
{
	BackgroundColorOverride = InBackgroundColor;
}

void SGINIEditableTextBox::SetReadOnlyForegroundColor(const TAttribute<FSlateColor>& InReadOnlyForegroundColor)
{
	ReadOnlyForegroundColorOverride = InReadOnlyForegroundColor;
}

void SGINIEditableTextBox::SetFocusedForegroundColor(const TAttribute<FSlateColor>& InFocusedForegroundColor)
{
	FocusedForegroundColorOverride = InFocusedForegroundColor;
}

void SGINIEditableTextBox::SetMaximumLength(const TAttribute<int32>& InMaximumLength)
{
	MaximumLength = InMaximumLength;
}

void SGINIEditableTextBox::SetMinimumLength(const TAttribute<int32>& InMinimumLength)
{
	MinimumLength = InMinimumLength;
}

void SGINIEditableTextBox::SetMinimumDesiredWidth(const TAttribute<float>& InMinimumDesiredWidth)
{
	EditableText->SetMinDesiredWidth(InMinimumDesiredWidth);
}


void SGINIEditableTextBox::SetIsCaretMovedWhenGainFocus(const TAttribute<bool>& InIsCaretMovedWhenGainFocus)
{
	EditableText->SetIsCaretMovedWhenGainFocus(InIsCaretMovedWhenGainFocus);
}


void SGINIEditableTextBox::SetSelectAllTextWhenFocused(const TAttribute<bool>& InSelectAllTextWhenFocused)
{
	EditableText->SetSelectAllTextWhenFocused(InSelectAllTextWhenFocused);
}


void SGINIEditableTextBox::SetRevertTextOnEscape(const TAttribute<bool>& InRevertTextOnEscape)
{
	EditableText->SetRevertTextOnEscape(InRevertTextOnEscape);
}


void SGINIEditableTextBox::SetClearKeyboardFocusOnCommit(const TAttribute<bool>& InClearKeyboardFocusOnCommit)
{
	EditableText->SetClearKeyboardFocusOnCommit(InClearKeyboardFocusOnCommit);
}


void SGINIEditableTextBox::SetSelectAllTextOnCommit(const TAttribute<bool>& InSelectAllTextOnCommit)
{
	EditableText->SetSelectAllTextOnCommit(InSelectAllTextOnCommit);
}

void SGINIEditableTextBox::SetSelectWordOnMouseDoubleClick(const TAttribute<bool>& InSelectWordOnMouseDoubleClick)
{
	EditableText->SetSelectWordOnMouseDoubleClick(InSelectWordOnMouseDoubleClick);
}

void SGINIEditableTextBox::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	EditableText->SetJustification(InJustification);
}


void SGINIEditableTextBox::SetAllowContextMenu(TAttribute<bool> InAllowContextMenu)
{
	EditableText->SetAllowContextMenu(InAllowContextMenu);
}

void SGINIEditableTextBox::SetVirtualKeyboardDismissAction(TAttribute<EVirtualKeyboardDismissAction> InVirtualKeyboardDismissAction)
{
	EditableText->SetVirtualKeyboardDismissAction(InVirtualKeyboardDismissAction);
}

void SGINIEditableTextBox::EnableTextInputMethodContext()
{
	EditableText->EnableTextInputMethodContext();
}
#if WITH_ACCESSIBILITY
TSharedRef<FSlateAccessibleWidget> SGINIEditableTextBox::CreateAccessibleWidget()
{
	return MakeShareable<FSlateAccessibleWidget>(new FSlateAccessibleEditableTextBox(SharedThis(this)));
}

TOptional<FText> SGINIEditableTextBox::GetDefaultAccessibleText(EAccessibleType AccessibleType) const
{
	// The parent Construct() function will call this before EditableText exists,
	// so we need a guard here to ignore that function call.
	if (EditableText.IsValid())
	{
		return EditableText->GetHintText();
	}
	return TOptional<FText>();
}
#endif

void SGINIEditableTextBox::OnEditableTextChanged(const FText& InText)
{
	OnTextChanged.ExecuteIfBound(InText);

	const int32 MaximumLengthValue = MaximumLength.Get();
	if (OnVerifyTextChanged.IsBound() || MaximumLengthValue >= 0)
	{
		FText OutErrorMessage;
		if (!VerifyLength(InText, OutErrorMessage) ||
			(OnVerifyTextChanged.IsBound() && !OnVerifyTextChanged.Execute(InText, OutErrorMessage)))
		{
			// Display as an error.
			SetError(OutErrorMessage);
		}
		else
		{
			SetError(FText::GetEmpty());
		}
	}
}

void SGINIEditableTextBox::OnEditableTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	FText OutErrorMessage;
	if (!VerifyLength(InText, OutErrorMessage) ||
		(OnVerifyTextChanged.IsBound() && !OnVerifyTextChanged.Execute(InText, OutErrorMessage)))
	{
		// Display as an error.
		if (InCommitType == ETextCommit::OnEnter)
		{
			SetError(OutErrorMessage);
		}
		return;
	}

	// Text commited without errors, so clear error text
	SetError(FText::GetEmpty());

	OnTextCommitted.ExecuteIfBound(InText, InCommitType);
}

void SGINIEditableTextBox::PlayHoverSound() const
{
	FSlateApplication::Get().PlaySound( HoveredSound );
}

void SGINIEditableTextBox::ExecuteHoverStateChanged(bool bPlaySound)
{
	if (IsHovered())
	{
		if (bPlaySound)
		{
			PlayHoverSound();
		}
		OnHovered.ExecuteIfBound();
	}
	else
	{
		OnUnhovered.ExecuteIfBound();
	}
}
*/
#undef LOCTEXT_NAMESPACE
