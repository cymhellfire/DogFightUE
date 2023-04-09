#include "SCreateLuaScriptWindow.h"

#include "DetailLayoutBuilder.h"
#include "Utils/DogFightUtilsFunctionLibrary.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE		"CreateScriptWindow"

SCreateLuaScriptWindow::SCreateLuaScriptWindow()
{
	bUseScriptPrefix = true;
	SelectedTab = ELuaScriptWindowTab::Default;
	SelectedTemplateMode = ELuaScriptNameTemplateMode::TM_None;

	DefaultRowPadding = FMargin(FVector4(5, 5, 0, 0));
}

void SCreateLuaScriptWindow::Construct(const FArguments& InArgs)
{
	ModuleNameList.Add(MakeShareable(new FString("DogFight")));
	ModuleNameList.Add(MakeShareable(new FString("GameFlow")));
	SelectedModule = ModuleNameList[0];
	TemplateNameList.Add(MakeShareable(new FString("Card")));
	TemplateNameList.Add(MakeShareable(new FString("CardModifier")));
	TemplateNameList.Add(MakeShareable(new FString("GameFlowState")));
	TemplateNameList.Add(MakeShareable(new FString("CardLogic")));
	TemplateNameList.Add(MakeShareable(new FString("CardAction")));
	SelectedTemplate = TemplateNameList[0];
	SetCreateTemplateList(*SelectedTemplate);

	for (int32 i = 0; i < 3; ++i)
	{
		ScriptNameTemplateModeOptions.Add(MakeShareable(new int32(i)));
	}
	ScriptNameTemplateModeDesc.Add(FString("None"));
	ScriptNameTemplateModeDesc.Add(FString("Prefix"));
	ScriptNameTemplateModeDesc.Add(FString("Suffix"));

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			ConstructTabHeaderList()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SWidgetSwitcher)
			.WidgetIndex_Lambda([this]()
			{
				return static_cast<int32>(SelectedTab);
			})
			+ SWidgetSwitcher::Slot()
			[
				ConstructDefaultTab()
			]
			+ SWidgetSwitcher::Slot()
			[
				ConstructMVVMTab()
			]
		]
	];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::ConstructTabHeaderList()
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString("Default"))
			.OnClicked_Lambda([this]()
			{
				SelectedTab = ELuaScriptWindowTab::Default;
				OnSwitchDefaultTab();
				return FReply::Handled();
			})
			.ForegroundColor_Lambda([this]()
			{
				return SelectedTab == ELuaScriptWindowTab::Default ? FLinearColor::White : FLinearColor::Gray;
			})
			.ButtonColorAndOpacity_Lambda([this]()
			{
				return SelectedTab == ELuaScriptWindowTab::Default ? FLinearColor::Gray : FLinearColor::Black;
			})
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString("MVVM"))
			.OnClicked_Lambda([this]()
			{
				SelectedTab = ELuaScriptWindowTab::MVVM;
				OnSwitchMVVMTab();
				return FReply::Handled();
			})
			.ForegroundColor_Lambda([this]()
			{
				return SelectedTab == ELuaScriptWindowTab::MVVM ? FLinearColor::White : FLinearColor::Gray;
			})
			.ButtonColorAndOpacity_Lambda([this]()
			{
				return SelectedTab == ELuaScriptWindowTab::MVVM ? FLinearColor::Gray : FLinearColor::Black;
			})
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::ConstructDefaultTab()
{
	return SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateModuleChooseSection()
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
			[
				CreateSubFolderSection()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateScriptNameSection()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreatePathPreviewSection()
			]
			+ SVerticalBox::Slot().FillHeight(1.f)		// Spacer
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateCreateFileSection()
			]
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::ConstructMVVMTab()
{
	return SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateModuleChooseSection()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateSubFolderSection()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateScriptNameSection()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreatePathPreviewSection()
			]
			+ SVerticalBox::Slot().FillHeight(1.f)		// Spacer
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateCreateFileSection()
			]
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::CreateModuleChooseSection()
{
	return SNew(SVerticalBox)
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
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::CreateSubFolderSection()
{
	return SNew(SVerticalBox)
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
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::CreateScriptNameSection()
{
	return SNew(SVerticalBox)
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
				SNew(STextBlock)
				.Text(LOCTEXT("TemplateMode", "Use template name as: "))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SComboBox<TSharedPtr<int32>>)
				.OptionsSource(&ScriptNameTemplateModeOptions)
				.OnGenerateWidget(this, &SCreateLuaScriptWindow::OnGeneratedScriptNameTemplateModeWidget)
				.OnSelectionChanged(this, &SCreateLuaScriptWindow::OnScriptNameTemplateModeSelectionChanged)
				.InitiallySelectedItem(ScriptNameTemplateModeOptions.Num() > 0 ? ScriptNameTemplateModeOptions[0] : nullptr)
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						int32 SelectIndex = (int32)SelectedTemplateMode;
						return FText::FromString(ScriptNameTemplateModeDesc[SelectIndex]);
					})
				]
			]
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::CreatePathPreviewSection()
{
	return SNew(SVerticalBox)
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
				.Text_Lambda([this]()
				{
					FString Result;
					for (int32 i = 0; i < CreateTemplateList.Num(); ++i)
					{
						Result += GetPreviewPathText(i);
						if (i < CreateTemplateList.Num() - 1)
						{
							Result += "\n";
						}
					}

					return FText::FromString(Result);
				})
			]
		];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::CreateCreateFileSection()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(DefaultRowPadding)
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

TSharedRef<SWidget> SCreateLuaScriptWindow::OnGeneratedScriptNameTemplateModeWidget(TSharedPtr<int32> InOption)
{
	return SNew(SBox)
	[
		SNew(STextBlock)
		.Text(FText::FromString(ScriptNameTemplateModeDesc[*InOption.Get()]))
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

	SetCreateTemplateList(*SelectedTemplate);
}

void SCreateLuaScriptWindow::OnScriptNameTemplateModeSelectionChanged(TSharedPtr<int32> InSelectedItem,
	ESelectInfo::Type SelectInfo)
{
	SelectedTemplateMode = (ELuaScriptNameTemplateMode)*InSelectedItem;
}

FReply SCreateLuaScriptWindow::OnCloseButtonClicked()
{
	TSharedRef<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared()).ToSharedRef();
	FSlateApplication::Get().RequestDestroyWindow(ParentWindow);

	return FReply::Handled();
}

FReply SCreateLuaScriptWindow::OnCreateButtonClicked()
{
	for (int32 i = 0; i < CreateTemplateList.Num(); ++i)
	{
		bLastCreateResult = UDogFightUtilsFunctionLibrary::CreateLuaScriptByTemplate(GetCurrentArgument(i));
	}

	return FReply::Handled();
}

FReply SCreateLuaScriptWindow::OnOpenFileButtonClicked()
{
	for (int32 i = 0; i < CreateTemplateList.Num(); ++i)
	{
		FString AbsolutePath = FPaths::ConvertRelativePathToFull(UDogFightUtilsFunctionLibrary::GenerateLuaScriptPath(
			GetCurrentArgument(i)));
	
		FPlatformProcess::LaunchFileInDefaultExternalApplication(*AbsolutePath);
	}

	return FReply::Handled();
}

void SCreateLuaScriptWindow::OnSwitchDefaultTab()
{
	OverrideFolder.Reset();
	HardcodedSubfolder.Reset();

	if (TemplateNameList.Num() > 0)
	{
		OnTemplateSelectionChanged(TemplateNameList[0], ESelectInfo::OnMouseClick);
	}

	if (ScriptNameTemplateModeOptions.Num() > 0)
	{
		OnScriptNameTemplateModeSelectionChanged(ScriptNameTemplateModeOptions[0], ESelectInfo::OnMouseClick);
	}
}

void SCreateLuaScriptWindow::OnSwitchMVVMTab()
{
	ClearCreateTemplateList();

	AddCreateTemplateList(TEXT("View"));
	AddCreateTemplateList(TEXT("VM"));

	HardcodedSubfolder = TEXT("Widget");
	OverrideFolder = TEXT("");

	if (ScriptNameTemplateModeOptions.Num() > 2)
	{
		OnScriptNameTemplateModeSelectionChanged(ScriptNameTemplateModeOptions[2], ESelectInfo::OnMouseClick);
	}
}

FLuaScriptCreateArgument SCreateLuaScriptWindow::GetCurrentArgument(int32 Index) const
{
	if (Index >= CreateTemplateList.Num())
	{
		return FLuaScriptCreateArgument();
	}

	FLuaScriptCreateArgument NewArgument;
	NewArgument.ModuleName = *SelectedModule;
	NewArgument.TemplateName = CreateTemplateList[Index];
	if (HardcodedSubfolder.IsSet())
	{
		if (ScriptPath.IsEmpty())
		{
			NewArgument.Path = HardcodedSubfolder.GetValue();
		}
		else
		{
			NewArgument.Path = HardcodedSubfolder.GetValue() / *ScriptPath.ToString();
		}
	}
	else
	{
		NewArgument.Path = ScriptPath.ToString();
	}
	NewArgument.ScriptName = NewScriptName.ToString();
	NewArgument.TemplateMode = SelectedTemplateMode;
	if (OverrideFolder.IsSet())
	{
		NewArgument.bOverrideTemplateFolder = true;
		NewArgument.OverrideFolder = OverrideFolder.GetValue();
	}

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

FString SCreateLuaScriptWindow::GetPreviewPathText(int32 Index) const
{
	return FPaths::ConvertRelativePathToFull(UDogFightUtilsFunctionLibrary::GenerateLuaScriptPath(
		GetCurrentArgument(Index)));
}

void SCreateLuaScriptWindow::SetCreateTemplateList(FString InTemplate)
{
	ClearCreateTemplateList();

	AddCreateTemplateList(InTemplate);
}

void SCreateLuaScriptWindow::AddCreateTemplateList(FString InTemplate)
{
	CreateTemplateList.Add(InTemplate);
}

void SCreateLuaScriptWindow::ClearCreateTemplateList()
{
	CreateTemplateList.Empty();
}

#undef LOCTEXT_NAMESPACE
