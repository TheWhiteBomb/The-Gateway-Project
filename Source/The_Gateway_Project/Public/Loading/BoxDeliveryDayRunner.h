// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoxDeliveryManager.h"
#include "Shared/DayCycleRunner.h"
#include "BoxDeliveryDayRunner.generated.h"

/**
 * 
 */
UCLASS()
class THE_GATEWAY_PROJECT_API ABoxDeliveryDayRunner : public ADayCycleRunner
{
	GENERATED_BODY()

public:
	virtual void OnDayChange() override;
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	ABoxDeliveryManager* BoxDeliveryManager;

	UFUNCTION()
	ABoxDeliveryManager* FindDeliveryManager();
};
