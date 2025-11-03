// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Loading/BoxDeliveryDayRunner.h"
#include "Engine/StaticMesh.h"
#include "GatewayConfiguration.h"

void ABoxDeliveryDayRunner::OnDayChange()
{
	BoxDeliveryManager->SpawnBoxesForDay(GetDayCount());
}

void ABoxDeliveryDayRunner::BeginPlay()
{
	Super::BeginPlay();
	BoxDeliveryManager = FindDeliveryManager();

	UE_LOG(GWLogDayChange, Display, TEXT("Ready to initialize box delivery outline."));
	BoxDeliveryManager->BoxDistributionPattern = BoxDeliveryManager->InitializeBoxDistribution();
	UE_LOG(GWLogDayChange, Display, TEXT("Finished box delivery outline initialization."));

	ProgressDaySequenceAtTime(0, 0.5f);
}

ABoxDeliveryManager* ABoxDeliveryDayRunner::FindDeliveryManager()
{
	TArray<AActor*> FoundDeliveryManagers;
	UGameplayStatics::GetAllActorsOfClass(this, ABoxDeliveryManager::StaticClass(), FoundDeliveryManagers);

	if (FoundDeliveryManagers.Num() <= 0)
	{
		UE_LOG(GWLogDayChange, Error, TEXT("No BoxDeliveryManagers found in level."))
		return nullptr;
	}
	
	return static_cast<ABoxDeliveryManager*>(FoundDeliveryManagers[0]);
}

