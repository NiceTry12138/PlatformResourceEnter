// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PREBrowserBinding.generated.h"

// 下载	
DECLARE_DELEGATE_TwoParams(FOnRSEDownload, FString, FString);

// 更新
DECLARE_DELEGATE_TwoParams(FOnRSEUpdate, FString, FString);

// 上传
DECLARE_DELEGATE_TwoParams(FOnRSEUpload, FString, FString);

/**
 * 
 */
UCLASS()
class UPREBrowserBinding : public UObject
{
	GENERATED_BODY()
	
};
