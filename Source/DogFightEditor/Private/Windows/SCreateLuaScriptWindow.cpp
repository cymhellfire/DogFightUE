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
	SelectedTemplateMode = ELuaScriptNameTemplateMode::TM_None;

	DefaultRowPadding = FMargin(FVector4(5, 5, 0, 0));
}

void SCreateLuaScriptWindow::Construct(const FArguments& InArgs)
{
	auto CreateLuaSettings = GetDefault<UCreateLuaScriptSettings>();
	if (CreateLuaSettings)
	{
		TabSettingList = CreateLuaSettings->TabList;
	}

	if (TabSettingList.Num() == 0)
	{
		ChildSlot[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Create lua setting invalid."))
			]
		];
		return;
	}

	for (int32 i = 0; i < 3; ++i)
	{
		ScriptNameTemplateModeOptions.Add(MakeShareable(new int32(i)));
	}
	ScriptNameTemplateModeDesc.Add(FString("None"));
	ScriptNameTemplateModeDesc.Add(FString("Prefix"));
	ScriptNameTemplateModeDesc.Add(FString("Suffix"));

	TSharedPtr<SWidgetSwitcher> TabContainer;
	SAssignNew(TabContainer, SWidgetSwitcher)
	.WidgetIndex_Lambda([this]()
	{
		return SelectTabIndex;
	});

	// Create tabs based on settings
	for (auto& TabSetting : TabSettingList)
	{
		if (TabSetting.Type == ECreateLuaScriptTabType::TT_Group)
		{
			TabContainer->AddSlot()
			[
				ConstructGroupTab()
			];
		}
		else
		{
			TabContainer->AddSlot()
			[
				ConstructDefaultTab()
			];
		}
	}

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
			TabContainer.ToSharedRef()
		]
	];

	SetSelectedTabIndex(0);
}

TSharedRef<SWidget> SCreateLuaScriptWindow::ConstructTabHeaderList()
{
	TSharedPtr<SHorizontalBox> TabContainer;
	SAssignNew(TabContainer, SHorizontalBox);

	if (TabSettingList.Num() > 0)
		SelectTabIndex = 0;

	for (int32 Index = 0; Index < TabSettingList.Num(); ++Index)
	{
		auto& TabSetting = TabSettingList[Index];
		TabContainer->AddSlot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString(TabSetting.TabName))
			.OnClicked_Lambda([this, Index]()
			{
				SetSelectedTabIndex(Index);
				return FReply::Handled();
			})
			.ForegroundColor_Lambda([this, Index]()
			{
				return SelectTabIndex == Index ? FLinearColor::White : FLinearColor::Gray;
			})
			.ButtonColorAndOpacity_Lambda([this, Index]()
			{
				return SelectTabIndex == Index ? FLinearColor::Gray : FLinearColor::Black;
			})
		];
	}

	return TabContainer.ToSharedRef();
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
					SNew(SComboBox<TSharedPtr<FCreateLuaScriptTemplateSettings>>)
					.OptionsSource(&TemplateSettingList)
					.OnGenerateWidget(this, &SCreateLuaScriptWindow::OnGeneratedTemplateOptionWidget)
					.OnSelectionChanged(this, &SCreateLuaScriptWindow::OnTemplateSelectionChanged)
					.InitiallySelectedItem(TemplateSettingList.Num() > 0 ? TemplateSettingList[0] : nullptr)
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							if (SelectedTemplate.IsValid())
							{
								return FText::FromString(SelectedTemplate->TemplateName);
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

TSharedRef<SWidget> SCreateLuaScriptWindow::ConstructGroupTab()
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
				SNew(SComboBox<TSharedPtr<FCreateLuaScriptModuleSettings>>)
				.OptionsSource(&ModuleSettingList)
				.OnGenerateWidget(this, &SCreateLuaScriptWindow::OnGeneratedModuleOptionWidget)
				.OnSelectionChanged(this, &SCreateLuaScriptWindow::OnModuleSelectionChanged)
				.InitiallySelectedItem(ModuleSettingList.Num() > 0 ? ModuleSettingList[0] : nullptr)
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						if (SelectedModule.IsValid())
						{
							return FText::FromString(SelectedModule->ModuleName);
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

TSharedRef<SWidget> SCreateLuaScriptWindow::OnGeneratedModuleOptionWidget(TSharedPtr<FCreateLuaScriptModuleSettings> InOption)
{
	return SNew(SBox)
	[
		SNew(STextBlock)
		.Text(FText::FromString(InOption->ModuleName))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	];
}

TSharedRef<SWidget> SCreateLuaScriptWindow::OnGeneratedTemplateOptionWidget(TSharedPtr<FCreateLuaScriptTemplateSettings> InOption)
{
	return SNew(SBox)
	[
		SNew(STextBlock)
		.Text(FText::FromString(InOption->TemplateName))
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

void SCreateLuaScriptWindow::OnModuleSelectionChanged(TSharedPtr<FCreateLuaScriptModuleSettings> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	if (!InSelectedItem.IsValid())
		return;

	SelectedModule = InSelectedItem;
}

void SCreateLuaScriptWindow::OnTemplateSelectionChanged(TSharedPtr<FCreateLuaScriptTemplateSettings> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	if (!InSelectedItem.IsValid())
		return;

	SelectedTemplate = InSelectedItem;

	SetCreateTemplateList(SelectedTemplate);
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

void SCreateLuaScriptWindow::SetSelectedTabIndex(int32 InValue)
{
	SelectTabIndex = InValue;

	ApplyTabSettings(InValue);
}

void SCreateLuaScriptWindow::ApplyTabSettings(int32 Index)
{
	ClearCreateTemplateList();

	auto& CurTabSetting = TabSettingList[Index];

	// Load module list
	ModuleSettingList.Empty();
	for (auto& ModuleSetting : CurTabSetting.ModuleList)
	{
		ModuleSettingList.Add(MakeShareable(new FCreateLuaScriptModuleSettings(ModuleSetting)));
	}
	if (ModuleSettingList.Num() > 0)
	{
		SelectedModule = ModuleSettingList[0];
	}

	// Load template list
	TemplateSettingList.Empty();
	for (auto& TemplateSetting : CurTabSetting.TemplateList)
	{
		TSharedPtr<FCreateLuaScriptTemplateSettings> NewSettings = MakeShareable(new FCreateLuaScriptTemplateSettings(TemplateSetting));
		TemplateSettingList.Add(NewSettings);
		if (CurTabSetting.Type == ECreateLuaScriptTabType::TT_Group)
		{
			AddCreateTemplateList(NewSettings);
		}
	}
	if (TemplateSettingList.Num() > 0)
	{
		if (CurTabSetting.Type == ECreateLuaScriptTabType::TT_Default)
		{
			SelectedTemplate = TemplateSettingList[0];
			SetCreateTemplateList(SelectedTemplate);
		}
	}

	// Set template name mode
	SelectedTemplateMode = CurTabSetting.DefaultTemplateNameMode;

	if (auto ParentWidget = GetParentWidget())
	{
		ParentWidget->Invalidate(EInvalidateWidgetReason::All);
	}
}

FLuaScriptCreateArgument SCreateLuaScriptWindow::GetCurrentArgument(int32 Index) const
{
	if (Index >= CreateTemplateList.Num())
	{
		return FLuaScriptCreateArgument();
	}

	FLuaScriptCreateArgument NewArgument;
	NewArgument.ModuleName = SelectedModule.IsValid() ? SelectedModule->ModuleName : "Untitled";
	auto& TemplateSettings = CreateTemplateList[Index];
	if (TemplateSettings.IsValid())
	{
		NewArgument.TemplateName = TemplateSettings->TemplateName;
		NewArgument.TemplateOutName = TemplateSettings->bOverrideShortName ? TemplateSettings->ShortName : TemplateSettings->TemplateName;
	}
	else
	{
		NewArgument.TemplateName = "NoTemplate";
	}
	auto& TabSettings = TabSettingList[SelectTabIndex];
	NewArgument.Path = ScriptPath.ToString();
	NewArgument.ScriptName = NewScriptName.ToString();
	NewArgument.TemplateMode = SelectedTemplateMode;
	
	if (TabSettings.Type == ECreateLuaScriptTabType::TT_Group)
	{
		NewArgument.bOverrideTemplateFolder = true;
		if (TabSettings.FolderNameType == ECreateLuaScriptGroupFolderNameType::FNT_Fixed)
		{
			NewArgument.OverrideFolder = TabSettings.OverrideSubfolder;
		}
		else
		{
			NewArgument.OverrideFolder = NewScriptName.ToString();
		}
	}
	else
	{
		NewArgument.bOverrideTemplateFolder = false;
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

void SCreateLuaScriptWindow::SetCreateTemplateList(TSharedPtr<FCreateLuaScriptTemplateSettings> InTemplate)
{
	ClearCreateTemplateList();

	AddCreateTemplateList(InTemplate);
}

void SCreateLuaScriptWindow::AddCreateTemplateList(TSharedPtr<FCreateLuaScriptTemplateSettings> InTemplate)
{
	CreateTemplateList.Add(InTemplate);
}

void SCreateLuaScriptWindow::ClearCreateTemplateList()
{
	CreateTemplateList.Empty();
}

#undef LOCTEXT_NAMESPACE
