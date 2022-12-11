#include "SCreateLuaScriptWindow.h"

#include "DetailLayoutBuilder.h"
#include "Utils/DogFightUtilsFunctionLibrary.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SSpacer.h"

#define LOCTEXT_NAMESPACE		"CreateScriptWindow"

SCreateLuaScriptWindow::SCreateLuaScriptWindow()
{
	bUseScriptPrefix = true;
}

void SCreateLuaScriptWindow::Construct(const FArguments& InArgs)
{
	ModuleNameList.Add(MakeShareable(new FString("DogFight")));
	ModuleNameList.Add(MakeShareable(new FString("GameFlow")));
	SelectedModule = ModuleNameList[0];
	TemplateNameList.Add(MakeShareable(new FString("Card")));
	TemplateNameList.Add(MakeShareable(new FString("CardModifier")));
	TemplateNameList.Add(MakeShareable(new FString("GameFlowState")));
	SelectedTemplate = TemplateNameList[0];

	const FMargin DefaultRowPadding(FVector4(5, 5, 0, 0));

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(DefaultRowPadding)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ModuleDesc", "Select the parent module new script belongs to."))
			]
			+ SVerticalBox::Slot()
			.Padding(DefaultRowPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Module", "Module: "))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&ModuleNameList)
					.OnGenerateWidget(this, &SCreateLuaScriptWindow::OnGeneratedModuleOptionWidget)
					.OnSelectionChanged(this, &SCreateLuaScriptWindow::OnModuleSelectionChanged)
					.InitiallySelectedItem(ModuleNameList.Num() > 0 ? ModuleNameList[0] : nullptr)
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							if (SelectedModule.IsValid())
							{
								return FText::FromString(*SelectedModule);
							}

							return FText::GetEmpty();
						})
					]
				]
			]
			+ SVerticalBox::Slot()
			.Padding(DefaultRowPadding)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("BasicDesc", "Select one of the template to create a new lua script."))
			]
			+ SVerticalBox::Slot()
			.Padding(DefaultRowPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Template", "Template: "))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&TemplateNameList)
					.OnGenerateWidget(this, &SCreateLuaScriptWindow::OnGeneratedTemplateOptionWidget)
					.OnSelectionChanged(this, &SCreateLuaScriptWindow::OnTemplateSelectionChanged)
					.InitiallySelectedItem(TemplateNameList.Num() > 0 ? TemplateNameList[0] : nullptr)
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							if (SelectedTemplate.IsValid())
							{
								return FText::FromString(*SelectedTemplate);
							}

							return FText::GetEmpty();
						})
					]
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(DefaultRowPadding)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FilePathDesc", "Specified the sub folder of new created script file. (Optional)"))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(DefaultRowPadding)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Path", "Sub Folder: "))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.FillWidth(1.f)
				[
					SNew(SEditableText)
					.HintText(LOCTEXT("PathHint", "Enter the sub folder name here..."))
					.OnTextCommitted(this, &SCreateLuaScriptWindow::OnPathTextCommitted)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(DefaultRowPadding)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ScriptNameDesc", "Give the new script a unique name."))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(DefaultRowPadding)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("NewName", "Name: "))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.FillWidth(1.f)
				[
					SNew(SEditableText)
					.HintText(LOCTEXT("NameHint", "Enter name for new script..."))
					.OnTextCommitted(this, &SCreateLuaScriptWindow::OnNameTextCommitted)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged_Lambda([this](ECheckBoxState InState)
					{
						bUseScriptPrefix = InState == ECheckBoxState::Checked;
						
					})
					.IsChecked_Lambda([this]()
					{
						return bUseScriptPrefix ? ECheckBoxState::Checked :ECheckBoxState::Unchecked;
					})
					[
						SNew(STextBlock)
						.Text(LOCTEXT("UsePrefix", "Use Prefix"))
					]
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(DefaultRowPadding)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("PreviewPath", "Preview File Path: "))
				]
				+ SHorizontalBox::Slot()
				.Padding(10.f, 0, 0, 0)
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(SEditableText)
					.IsReadOnly(true)
					.Text(this, &SCreateLuaScriptWindow::GetPreviewPathText)
				]
			]
			+ SVerticalBox::Slot().FillHeight(1.f)		// Spacer
			+ SVerticalBox::Slot()
			.Padding(DefaultRowPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						if (bLastCreateResult.IsSet())
						{
							bool bSuccess = bLastCreateResult.GetValue() == "Success"; 
							return bSuccess ? LOCTEXT("Success", "Success") : FText::FromString(bLastCreateResult.GetValue());
						}

						return FText::GetEmpty();
					})
					.ColorAndOpacity_Lambda([this]()
					{
						if (bLastCreateResult.IsSet())
						{
							bool bSuccess = bLastCreateResult.GetValue() == "Success"; 
							return bSuccess ? FLinearColor::Green : FLinearColor::Red;
						}

						return FLinearColor::Red;
					})
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(5, 0, 5, 5)
				.FillWidth(1.f)
				[
					SNew(SButton)
					.Text(LOCTEXT("OpenFile", "Open"))
					.Visibility_Lambda([this]()
					{
						if (bLastCreateResult.IsSet())
						{
							bool bSuccess = bLastCreateResult.GetValue() == "Success";
							return bSuccess ? EVisibility::Visible : EVisibility::Collapsed;
						}

						return EVisibility::Collapsed;
					})
					.OnClicked(this, &SCreateLuaScriptWindow::OnOpenFileButtonClicked)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.Padding(0, 0, 5, 5)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.Text(LOCTEXT("Create", "Create"))
						.OnClicked(this, &SCreateLuaScriptWindow::OnCreateButtonClicked)
						.IsEnabled_Lambda([this]()
						{
							return !NewScriptName.IsEmpty();
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.Text(LOCTEXT("Close", "Close"))
						.OnClicked(this, &SCreateLuaScriptWindow::OnCloseButtonClicked)
					]
				]
			]
		]
	];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::OnGeneratedModuleOptionWidget(TSharedPtr<FString> InOption)
{
	return SNew(SBox)
	[
		SNew(STextBlock)
		.Text(FText::FromString(*InOption))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::OnGeneratedTemplateOptionWidget(TSharedPtr<FString> InOption)
{
	return SNew(SBox)
	[
		SNew(STextBlock)
		.Text(FText::FromString(*InOption))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	];
}

void SCreateLuaScriptWindow::OnModuleSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	SelectedModule = InSelectedItem;
}

void SCreateLuaScriptWindow::OnTemplateSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	SelectedTemplate = InSelectedItem;
}

FReply SCreateLuaScriptWindow::OnCloseButtonClicked()
{
	TSharedRef<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared()).ToSharedRef();
	FSlateApplication::Get().RequestDestroyWindow(ParentWindow);

	return FReply::Handled();
}

FReply SCreateLuaScriptWindow::OnCreateButtonClicked()
{
	bLastCreateResult = UDogFightUtilsFunctionLibrary::CreateLuaScriptByTemplate(GetCurrentArgument());

	return FReply::Handled();
}

FReply SCreateLuaScriptWindow::OnOpenFileButtonClicked()
{
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(UDogFightUtilsFunctionLibrary::GenerateLuaScriptPath(
		GetCurrentArgument()));
	
	FPlatformProcess::LaunchFileInDefaultExternalApplication(*AbsolutePath);

	return FReply::Handled();
}

FLuaScriptCreateArgument SCreateLuaScriptWindow::GetCurrentArgument() const
{
	FLuaScriptCreateArgument NewArgument;
	NewArgument.ModuleName = *SelectedModule;
	NewArgument.TemplateName = *SelectedTemplate;
	NewArgument.Path = ScriptPath.ToString();
	NewArgument.ScriptName = NewScriptName.ToString();
	NewArgument.bUsePrefix = bUseScriptPrefix;

	return NewArgument;
}

void SCreateLuaScriptWindow::OnPathTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	ScriptPath = InText;
}

void SCreateLuaScriptWindow::OnNameTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	NewScriptName = InText;
}

FText SCreateLuaScriptWindow::GetPreviewPathText() const
{
	return FText::FromString(FPaths::ConvertRelativePathToFull(UDogFightUtilsFunctionLibrary::GenerateLuaScriptPath(
		GetCurrentArgument())));
}

#undef LOCTEXT_NAMESPACE
