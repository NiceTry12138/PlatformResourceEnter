// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IWebBrowserSingleton.h"

class UPREBrowserBinding;
class SDockTab;
class SWebBrowser;
class SWindow;

/**
 *
 */
class FPREUIManagerImpl : public TSharedFromThis<FPREUIManagerImpl>
{
public:
	void Initialize();
	void Shutdown();

protected:
	void SetupMenuItem();
	void CreateWindow();
	TSharedRef<SDockTab> CreateWindowTab(const FSpawnTabArgs& Args);

protected:
	const FText PRETabDisplay = FText::FromString("Platform Resource Enter");
	const FText PREToolTip = FText::FromString("Launch Platform Resource Enter");

public:
	FCreateBrowserWindowSettings WindowSettings;
	TSharedPtr<IWebBrowserWindow> Browser;
	TSharedPtr<SWebBrowser> WebBrowserWidget;
	TSharedPtr<SDockTab> LocalBrowserDock;
};

/**
 * 
 */
class FPREUIManager
{
public:
	static void Initialize();
	static void Shutdown();

	static TSharedPtr<FPREUIManagerImpl> Instance;
	static UPREBrowserBinding* PREBrowserBinding;
};
