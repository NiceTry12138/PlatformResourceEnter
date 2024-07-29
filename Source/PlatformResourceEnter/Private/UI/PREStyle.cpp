// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PREStyle.h"

#include "Brushes/SlateImageBrush.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/Paths.h"
#include "Styling/SlateStyle.h"
#include "Rendering/SlateRenderer.h"
#include "Styling/SlateStyleRegistry.h"

const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TUniquePtr<FSlateStyleSet> FPREStyle::MSStyleInstance;

void FPREStyle::Initialize()
{
	if (!MSStyleInstance.IsValid())
	{
		MSStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*MSStyleInstance);
	}
}

void FPREStyle::Shutdown()
{
	if (MSStyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*MSStyleInstance);
		MSStyleInstance.Reset();
	}
}

const ISlateStyle& FPREStyle::Get()
{
	check(MSStyleInstance);
	return *MSStyleInstance;
}

FName FPREStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("PREStyle"));
	return StyleSetName;
}

FName FPREStyle::GetContextName()
{
	return FName(TEXT("PRE"));
}

void FPREStyle::SetIcon(const FString& StyleName, const FString& ResourcePath)
{
	FSlateStyleSet* Style = MSStyleInstance.Get();

	FString Name(GetContextName().ToString());
	Name = Name + "." + StyleName;
	Style->Set(*Name, new FSlateImageBrush(Style->RootToContentDir(ResourcePath, TEXT(".png")), Icon40x40));

	Name += ".Small";
	Style->Set(*Name, new FSlateImageBrush(Style->RootToContentDir(ResourcePath, TEXT(".png")), Icon20x20));

	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

void FPREStyle::SetSVGIcon(const FString& StyleName, const FString& ResourcePath)
{
	FSlateStyleSet* Style = MSStyleInstance.Get();

	FString Name(GetContextName().ToString());
	Name = Name + "." + StyleName;
	Style->Set(*Name, new FSlateVectorImageBrush(Style->RootToContentDir(ResourcePath, TEXT(".svg")), Icon40x40));

	Name += ".Small";
	Style->Set(*Name, new FSlateVectorImageBrush(Style->RootToContentDir(ResourcePath, TEXT(".svg")), Icon20x20));

	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

TUniquePtr<FSlateStyleSet> FPREStyle::Create()
{
	TUniquePtr< FSlateStyleSet > Style = MakeUnique<FSlateStyleSet>(GetStyleSetName());
	Style->SetContentRoot(FPaths::EnginePluginsDir() / TEXT("PlatformResourceEnter/Resources"));
	return Style;
}
