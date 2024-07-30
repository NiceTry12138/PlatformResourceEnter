// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PREBrowserBinding.generated.h"
struct FWebJSFunction;

// ����	
DECLARE_DELEGATE_TwoParams(FOnPREDownload, FString, FString);

// ����
DECLARE_DELEGATE_TwoParams(FOnPREUpdate, FString, FString);

// �ϴ�
DECLARE_DELEGATE_TwoParams(FOnPREUpload, FString, FString);

// �˳�
DECLARE_DELEGATE_OneParam(FOnPREExit, FString);

/**
 * 
 */
UCLASS()
class UPREBrowserBinding : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	FOnPREExit OnPREExitDelegate;

public:
	UFUNCTION()
	void OnExitCallback(FWebJSFunction ObExitJSCallback);

	UFUNCTION()
	void OnDownLoadButtonClick(FString Param1, int Param2);
};
