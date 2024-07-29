// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IWebBrowserSingleton.h"

class URSEBrowserBinding;

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
	static URSEBrowserBinding* BrowserBinding;
};
