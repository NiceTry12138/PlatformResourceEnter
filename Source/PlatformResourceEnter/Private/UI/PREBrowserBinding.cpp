// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PREBrowserBinding.h"
#include "WebJSFunction.h"

UPREBrowserBinding::UPREBrowserBinding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UPREBrowserBinding::OnExitCallback(FWebJSFunction ObExitJSCallback)
{
	OnPREExitDelegate.BindLambda(ObExitJSCallback);
}

void UPREBrowserBinding::OnDownLoadButtonClick(FString Param1, int Param2)
{
	UE_LOG(LogTemp, Log, TEXT("11111111111111 %d %s"), Param2, *Param1);
}
