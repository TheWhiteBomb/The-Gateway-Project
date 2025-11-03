// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PointLight.h"
#include "Shared/DayProgressController.h"
#include "BoxDeliveryProgressController.generated.h"

/**
 * 
 */
UCLASS()
class THE_GATEWAY_PROJECT_API ABoxDeliveryProgressController : public ADayProgressController
{
	GENERATED_BODY()

public:
	virtual void OnPreTransition() override;
	virtual void OnTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightDisableTransitionAlpha = 0.3f;
	
	virtual void OnDestroyAnyReason() override;

private:
	UFUNCTION()
	TArray<AActor*> GetPointLights();

	UPROPERTY()
	TArray<AActor*> PointLights;
};
