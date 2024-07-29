// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PREUIManager.h"
#include "UI/PREStyle.h"
#include "UI/PREBrowserBinding.h"
#include "LevelEditor.h"

// ToolMenus
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "ToolMenuEntry.h"
#include "ToolMenuSection.h"

TSharedPtr<FPREUIManagerImpl> FPREUIManager::Instance = nullptr;

#define LEVELEDITOR_MODULE_NAME TEXT("LevelEditor")

const FName RPETabName = "RPETabName";

void FPREUIManager::Initialize()
{
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new FPREUIManagerImpl);
		Instance->Initialize();
	}
}

void FPREUIManager::Shutdown()
{
	if (Instance.IsValid()) {
		Instance->Shutdown();
	}
}

void FPREUIManagerImpl::Initialize()
{
	FPREStyle::Initialize();
	SetupMenuItem();
}

void FPREUIManagerImpl::Shutdown()
{
	FPREStyle::Shutdown();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(RPETabName);
}

void FPREUIManagerImpl::SetupMenuItem()
{
	FPREStyle::SetIcon("Logo", "Logo80x80");
	FPREStyle::SetIcon("ContextLogo", "Logo32x32");
	FPREStyle::SetSVGIcon("MenuLogo", "PREB");

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LEVELEDITOR_MODULE_NAME);
	
	UToolMenu* UserMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
	FToolMenuSection& Section = UserMenu->FindOrAddSection("Content");
	Section.AddMenuEntry("OpenPRETab",
		LOCTEXT("OpenPRETab_Label", "Platform Res Enter"),
		LOCTEXT("OpenPRETab_Desc", "Opens Platform Resource Enter."),
		FSlateIcon(FPREStyle::GetStyleSetName(), "PRE.MenuLogo"),
		FUIAction(FExecuteAction::CreateRaw(this, &FPREUIManagerImpl::CreateWindow), FCanExecuteAction())
	).InsertPosition = FToolMenuInsert("ImportContent", EToolMenuInsertType::After);
}

void FPREUIManagerImpl::CreateWindow()
{
}
