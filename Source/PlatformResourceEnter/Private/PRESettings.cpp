// Fill out your copyright notice in the Description page of Project Settings.


#include "PRESettings.h"

UPRESettings::UPRESettings(const FObjectInitializer& Initializer)
{
	WindowsSize = FVector2D(800, 500);
}

UPRESettings* UPRESettings::GetSettings()
{
	return GetMutableDefault<UPRESettings>();
}
