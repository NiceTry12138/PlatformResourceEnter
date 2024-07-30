// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlatformResourceEnterModule.h"
#include "UI/PREUIManager.h"

#define LOCTEXT_NAMESPACE "PlatformResourceEnterModule"

void FPlatformResourceEnterModule::StartupModule()
{
	FPREUIManager::Initialize();
}

void FPlatformResourceEnterModule::ShutdownModule()
{
	FPREUIManager::Shutdown();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPlatformResourceEnterModule, PlatformResourceEnterEditorMode)