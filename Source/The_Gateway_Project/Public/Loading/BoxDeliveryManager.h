// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Private/Loading/BoxDeliveryOutline.h"
#include "../../Private/Loading/BoxDeliveryDay.h"
#include "BoxDeliveryManager.generated.h"

UCLASS()
class THE_GATEWAY_PROJECT_API ABoxDeliveryManager : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	USceneComponent* _RootComponent;

	UFUNCTION()
	int GetOverallBoxCount(TArray<FDeliveryElementConfig> boxConfigs);

	UFUNCTION()
	TArray<FDeliveryElementConfig> GetDayOfBoxes(int totalBoxesOfDay, TArray<FDeliveryElementConfig> configs, TArray<int>& sentAmounts, bool getEverything);

	UPROPERTY()
	int RandomBoxRecursionCounter = 0;

	UPROPERTY()
	TArray<AActor*> Boxes;
	
public:
	UPROPERTY(EditAnywhere, Category="Delivery Configuration")
	float BoxSpawnDistributionDistance = 150;
	
	UPROPERTY(EditAnywhere, Category="Delivery Configuration", DisplayName="Box Classes")
	TMap<EBoxSize, TSubclassOf<AActor>> StaticBoxParentClasses;
	
	UFUNCTION()
	TArray<FBoxDeliveryDay> InitializeBoxDistribution();

	UFUNCTION(BlueprintCallable)
	void SpawnBoxesForDay(int day);

	UPROPERTY()
	TArray<FBoxDeliveryDay> BoxDistributionPattern;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Delivery Configuration")
	UDataTable* BoxDeliveryData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Delivery Configuration")
	int TotalDays = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shuffle Configuration")
	int SelectionPercentageBias = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shuffle Configuration")
	bool ReverseBoxSelectionMethod = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shuffle Configuration")
	int RandomBoxRecursionBound = 1000;
	
protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	ABoxDeliveryManager();
};