// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DayProgressController.generated.h"

/**
 * 
 */
UCLASS()
class THE_GATEWAY_PROJECT_API ADayProgressController : public AActor
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void OnPreTransitionFinished() {}

public:
	UFUNCTION()
	virtual void OnPreTransition() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool TransitionFinalized;

	UPROPERTY()
	bool AccountedTransition;
	
	UFUNCTION(BlueprintCallable)
	void FinalizeSequence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionDuration = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TransitionProgress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ResetDaysOnComplete = false;
	
	UFUNCTION()
	virtual void OnTick(float DeltaTime);

	UFUNCTION()
	virtual void OnParentRecognizedTransitionFinalized() {}

	UFUNCTION()
	virtual void OnDestroyAnyReason() {}
};
