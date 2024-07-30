// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PREUIManager.h"
#include "UI/PREStyle.h"
#include "UI/PREBrowserBinding.h"
#include "LevelEditor.h"

#include "PRESettings.h"

// ToolMenus
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "ToolMenuEntry.h"
#include "ToolMenuSection.h"

// WebBrowser
#include "SWebBrowser.h"
#include "Serialization/JsonSerializer.h"
#include "WebBrowserModule.h"
#include "IWebBrowserWindow.h"

#define LEVELEDITOR_MODULE_NAME TEXT("LevelEditor")
#define LOCTEXT_NAMESPACE "PRE"

TSharedPtr<FPREUIManagerImpl> FPREUIManager::Instance = nullptr;
UPREBrowserBinding* FPREUIManager::PREBrowserBinding = nullptr;

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
	if (FPREUIManager::Instance.IsValid())
	{
		if (FPREUIManager::Instance->Browser != nullptr && FPREUIManager::Instance->Browser.IsValid())
		{
			FPREUIManager::Instance->Browser = nullptr;
		}
		if (FPREUIManager::Instance->WebBrowserWidget != nullptr && FPREUIManager::Instance->WebBrowserWidget.IsValid())
		{
			FPREUIManager::Instance->WebBrowserWidget = nullptr;
		}
		if (FPREUIManager::Instance->LocalBrowserDock != nullptr && FPREUIManager::Instance->LocalBrowserDock.IsValid())
		{
			FPREUIManager::Instance->LocalBrowserDock = nullptr;
		}
	}

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

	TArray<TSharedRef<SWindow>> Windows = FSlateApplication::Get().GetTopLevelWindows();
	if (Windows.Num() > 0)
	{
		// 调整窗口位置和大小
		FVector2D MainWindowSize = Windows[0]->GetSizeInScreen();
		float DesiredWidth = UPRESettings::GetSettings()->WindowsSize.X;
		float DesiredHeight = UPRESettings::GetSettings()->WindowsSize.Y;

		if (DesiredWidth < MainWindowSize.X && DesiredHeight < MainWindowSize.Y && LocalBrowserDock->GetParentWindow().IsValid())
		{
			// LocalBrowserDock->GetParentWindow() 返回的是包含标签页的窗口， 可能是主窗口 也可能是独立的窗口
			// windows[0] 通常是主窗口，如果 LocalBrowserDock 就是主窗口，表示 LocalBrowserDock 停靠在主窗口中
			if (LocalBrowserDock->GetParentWindow() == Windows[0])
			{
				return;
			}

			LocalBrowserDock->GetParentWindow()->Resize(FVector2D(DesiredWidth, DesiredHeight));
			LocalBrowserDock->GetParentWindow()->MoveWindowTo(FVector2D((MainWindowSize.X - DesiredWidth) - 17, MainWindowSize.Y - DesiredHeight) / 2);
		}
	}
}

TSharedRef<SDockTab> FPREUIManagerImpl::CreateWindowTab(const FSpawnTabArgs& Args)
{
	FString PluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("PlatformResourceEnter"));
	FString IndexUrl = FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginPath, TEXT("ThirdParty"), TEXT("web"), TEXT("index.html")));

	FString FinalUrl = FPaths::Combine(TEXT("file:///"), IndexUrl);

	TSharedPtr<SWebBrowser> PluginWebBrowser;

#if PLATFORM_MAC
	WindowSettings.InitialURL = FinalUrl;
	IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
	Browser = WebBrowserSingleton->CreateBrowserWindow(WindowSettings);
	PluginWebBrowser = SAssignNew(WebBrowserWidget, SWebBrowser, Browser).ShowAddressBar(false).ShowControls(false);
#elif PLATFORM_WINDOWS || PLATFORM_LINUX
	FWebBrowserInitSettings browserInitSettings = FWebBrowserInitSettings();
	IWebBrowserModule::Get().CustomInitialize(browserInitSettings);
	WindowSettings.InitialURL = FinalUrl;
	WindowSettings.BrowserFrameRate = 60;
	if (IWebBrowserModule::IsAvailable() && IWebBrowserModule::Get().IsWebModuleAvailable())
	{
		IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
		Browser = WebBrowserSingleton->CreateBrowserWindow(WindowSettings);
		PluginWebBrowser = SAssignNew(WebBrowserWidget, SWebBrowser, Browser)
			.ShowAddressBar(false)
			.ShowControls(false);
	}
	else
	{
		// 插件依赖提醒
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("PlatformResourceEnter requires the “Web Browser” plugin, which is disabled. Go to Edit > Plugins and search for “Web Browser” to enable it.")), FText::FromString(TEXT("Enable Web Browser Plugin")));
		return SAssignNew(LocalBrowserDock, SDockTab).TabRole(ETabRole::NomadTab);
	}
#endif

	// 处理 WebBrowser 小部件中未处理的按键事件，以防止这些事件影响 Unreal Engine 编辑器的状态
	// 通过绑定这些 lambda 函数，所有未处理的按键事件都会被标记为已处理，否则点击事件会穿透到编辑器界面
	Browser->OnUnhandledKeyUp().BindLambda([](const FKeyEvent&) { return true; });
	Browser->OnUnhandledKeyDown().BindLambda([](const FKeyEvent&) { return true; });

	// 创建并返回一个新的 SDockTab
	SAssignNew(LocalBrowserDock, SDockTab)
		.OnTabClosed_Lambda([](TSharedRef<class SDockTab> InParentTab)
			{
				// 绑定结束事件
				FPREUIManager::PREBrowserBinding->FOnPREExitDelegate.ExecuteIfBound("Plugin Window Closed");
				FPREUIManager::PREBrowserBinding = nullptr;

				// 清理缓存
				FPREUIManager::Instance->LocalBrowserDock = nullptr;
				if (FPREUIManager::Instance->WebBrowserWidget.IsValid())
				{
					FPREUIManager::Instance->WebBrowserWidget.Reset();
					FPREUIManager::Instance->Browser.Reset();
				}
			})
		.TabRole(ETabRole::NomadTab)
				[
					PluginWebBrowser.ToSharedRef()
				];

	// Browser 负责管理浏览器窗口的生命周期和操作，例如加载 URL、执行 JavaScript、处理浏览器事件等。Browser 是 WebBrowserWidget 的底层实现，提供实际的浏览器功能
	// WebBrowserWidget 提供了在 Unreal Engine 中嵌入浏览器的功能，允许在游戏或应用程序中显示网页内容
	// LocalBrowserDock 用于在 Unreal Engine 编辑器中创建一个新的标签页，并将 WebBrowserWidget 嵌入其中

	if (WebBrowserWidget.IsValid())
	{
		// 绑定对象 用于接收 JS 事件
		FPREUIManager::PREBrowserBinding = NewObject<UPREBrowserBinding>();
		FPREUIManager::Instance->WebBrowserWidget->BindUObject(TEXT("BrowserBinding"), FPREUIManager::PREBrowserBinding, true);
	}

	if (LocalBrowserDock.IsValid())
	{
		return LocalBrowserDock.ToSharedRef();
	}

	return SAssignNew(LocalBrowserDock, SDockTab);
}
