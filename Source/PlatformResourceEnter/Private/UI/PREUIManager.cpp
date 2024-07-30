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

#define LEVELEDITOR_MODULE_NAME TEXT("LevelEditor")
#define LOCTEXT_NAMESPACE "PRE"

TSharedPtr<FPREUIManagerImpl> FPREUIManager::Instance = nullptr;

const FName PRETabName = "RPETabName";

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
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PRETabName);
}

void FPREUIManagerImpl::SetupMenuItem()
{
	FPREStyle::SetIcon("Logo", "Logo80x80");
	FPREStyle::SetIcon("ContextLogo", "Logo32x32");
	FPREStyle::SetSVGIcon("MenuLogo", "PREB");

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LEVELEDITOR_MODULE_NAME);
	
	UToolMenu* UserMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = UserMenu->FindOrAddSection("PluginTools");
	Section.AddEntry(FToolMenuEntry::InitToolBarButton("PRE Button",
		FUIAction(FExecuteAction::CreateRaw(this, &FPREUIManagerImpl::CreateWindow)),
		TAttribute<FText>(),
		TAttribute<FText>(),
		FSlateIcon(FPREStyle::GetStyleSetName(), "PRE.MenuLogo")
	));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PRETabName,
		FOnSpawnTab::CreateRaw(this, &FPREUIManagerImpl::CreateWindowTab))
		.SetDisplayName(PRETabDisplay).SetAutoGenerateMenuEntry(false).SetTooltipText(PREToolTip);
}

void FPREUIManagerImpl::CreateWindow()
{
	FGlobalTabmanager::Get()->TryInvokeTab(PRETabName);

	//// Set desired window size (if the desired window size is less than main window size)
	//// Rationale: the main window is mostly maximized - so the size is equal to screen size
	//TArray<TSharedRef<SWindow>> Windows = FSlateApplication::Get().GetTopLevelWindows();
	//if (Windows.Num() > 0)
	//{
	//	FVector2D MainWindowSize = Windows[0]->GetSizeInScreen();
	//	float DesiredWidth = 1650;
	//	float DesiredHeight = 900;

	//	if (DesiredWidth < MainWindowSize.X && DesiredHeight < MainWindowSize.Y && LocalBrowserDock->GetParentWindow().IsValid())
	//	{
	//		// If Bridge is docked as a tab, the parent window will be the main window
	//		if (LocalBrowserDock->GetParentWindow() == Windows[0])
	//		{
	//			return;
	//		}

	//		LocalBrowserDock->GetParentWindow()->Resize(FVector2D(DesiredWidth, DesiredHeight));
	//		LocalBrowserDock->GetParentWindow()->MoveWindowTo(FVector2D((MainWindowSize.X - DesiredWidth) - 17, MainWindowSize.Y - DesiredHeight) / 2);
	//	}
	//}
}

TSharedRef<SDockTab> FPREUIManagerImpl::CreateWindowTab(const FSpawnTabArgs& Args)
{
	FString PluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("PlatformResourceEnter"));
	FString IndexUrl = FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginPath, TEXT("ThirdParty"), TEXT("web"), TEXT("index.html")));
}
