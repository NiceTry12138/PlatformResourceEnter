// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PRESettings.generated.h"

/**
 * 
 */
UCLASS(config = PRESetting, defaultconfig)
class PLATFORMRESOURCEENTER_API UPRESettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPRESettings(const FObjectInitializer& Initializer);

	static UPRESettings* GetSettings();

public:
	/**
	* ´°¿Ú³ß´ç
	*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = PRESetting)
	FVector2D WindowsSize;
};
