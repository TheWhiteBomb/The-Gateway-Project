// Fill out your copyright notice in the Description page of Project Settings.


#include "Loading/BoxDeliveryManager.h"

#include "GatewayConfiguration.h"

// Sets default values
ABoxDeliveryManager::ABoxDeliveryManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(_RootComponent);
}

// Called when the game starts or when spawned
void ABoxDeliveryManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoxDeliveryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ABoxDeliveryManager::GetOverallBoxCount(TArray<FDeliveryElementConfig> boxConfigs)
{
	int count = 0;
	for (auto element : boxConfigs)
	{
		count += element.TotalAmount;
	}
	return count;
}

TArray<FBoxDeliveryDay> ABoxDeliveryManager::InitializeBoxDistribution()
{
	TArray<FBoxDeliveryDay> boxDistribution;
	TArray<FDeliveryElementConfig> outBoxConfigs;

	TArray<int> sentAmounts;
	
	TArray<FDeliveryElementConfig*> outBoxConfigsPtrArray;
	BoxDeliveryData->GetAllRows<FDeliveryElementConfig>(TEXT("Searching for Box Configs"), outBoxConfigsPtrArray);
	
	for (int i = 0; i < outBoxConfigsPtrArray.Num(); i++)
	{
		auto data = outBoxConfigsPtrArray[i];
		data->SentAmount = 0;
		outBoxConfigs.Add(*data);

		sentAmounts.Add(0);
	}
	
	auto reverseBoxSelectionMethod = ReverseBoxSelectionMethod;
	outBoxConfigs.Sort([&reverseBoxSelectionMethod](FDeliveryElementConfig a, FDeliveryElementConfig b)
	{
		return reverseBoxSelectionMethod ? a.ArrivalPriority > b.ArrivalPriority : a.ArrivalPriority < b.ArrivalPriority;
	});
	
	static int boxCount = GetOverallBoxCount(outBoxConfigs);
	static float averageBoxesPerDay = static_cast<float>(boxCount) / static_cast<float>(TotalDays);

	static int boxesPerDay = FMath::FloorToInt(averageBoxesPerDay);
	
	float remainderTracker = 0;
	int totalBoxCounter = 0;
	TArray<int> boxCounts;
	for (int i = 0; i < TotalDays; i++)
	{
		int nextDayCount = boxesPerDay;

		bool lastDay = i == TotalDays - 1;
		if (!lastDay)
		{
			remainderTracker += averageBoxesPerDay-boxesPerDay;
			int remainderTrackerChange = FMath::Floor(remainderTracker);
			remainderTracker -= remainderTrackerChange;

			nextDayCount += remainderTrackerChange;
			UE_LOG(GWLogBoxManager, Display, TEXT("Remainder Tracker: %f"), remainderTracker);
		}
		else
		{
			nextDayCount = boxCount - totalBoxCounter;
		}
		
		FBoxDeliveryDay nextDay;
		nextDay.Value = GetDayOfBoxes(nextDayCount, outBoxConfigs, sentAmounts, lastDay);
		RandomBoxRecursionCounter = 0;
		
		boxDistribution.Add(nextDay);
		totalBoxCounter += nextDayCount;
		UE_LOG(GWLogBoxManager, Display, TEXT("Day result: %d/%d. (%d) Avg: %f"), nextDayCount, boxCount, totalBoxCounter, averageBoxesPerDay);
	}

	return boxDistribution;
}

TArray<FDeliveryElementConfig> ABoxDeliveryManager::GetDayOfBoxes(int totalBoxesOfDay, TArray<FDeliveryElementConfig> configs, TArray<int>& sentAmounts, bool getEverything)
{
	RandomBoxRecursionCounter++;
	TArray<FDeliveryElementConfig> resultBoxes;

	float recursionChanceBias = static_cast<float>(RandomBoxRecursionCounter)/static_cast<float>(RandomBoxRecursionBound);
	
	for (int i = 0; i < totalBoxesOfDay; i++)
	{
		for (auto k = 0; k < configs.Num(); k++)
		{
			auto config = configs[k];
			int isLastIteration = k >= configs.Num() - 1;

			// Generally, this repeats a lot less than RandomBoxRecursionBound. The reason we need this
			// is to make sure that it's statistically unlikely to get the wrong types of boxes. Either way,
			// the correct boxes should arrive once getEverything enables.
			if (sentAmounts[k] >= config.TotalAmount && RandomBoxRecursionCounter < RandomBoxRecursionBound)
			{
				if (isLastIteration) { return GetDayOfBoxes(totalBoxesOfDay, configs, sentAmounts, getEverything); }
				continue;
			}

			if (isLastIteration || getEverything || FMath::FRand() < static_cast<float>(config.ArrivalPriority + 1)/100.0f + recursionChanceBias)
			{
				sentAmounts[k]++;
				resultBoxes.Add(config);
				UE_LOG(GWLogBoxManager, Display, TEXT("Successfully added new box. Box Name: %s (%d/%d)"), *config.DisplayName.ToString(), sentAmounts[k], config.TotalAmount);
				break;
			}
		}
	}

	return resultBoxes;
}

void ABoxDeliveryManager::SpawnBoxesForDay(int day)
{
	TArray<FDeliveryElementConfig> todayBoxes = BoxDistributionPattern[day].Value;
	UWorld* World = GetWorld();

	const float circleMaximum = todayBoxes.Num();
	
	for (int i = 0; i < circleMaximum; i++)
	{
		FDeliveryElementConfig v = todayBoxes[i];

		const float circleCurrent = static_cast<float>(i)/circleMaximum;
		const float appliedCirclePosition = 2*PI * circleCurrent;

		FVector positionOffset = FVector(
			FMath::Cos(appliedCirclePosition),
			FMath::Sin(appliedCirclePosition),
			0
		) * BoxSpawnDistributionDistance;
		
		FRotator rotationOffset = FRotator(0, FMath::FRand() * 360, 0);

		const FVector location = GetActorLocation() + positionOffset;
		const FRotator rotation = GetActorRotation() + rotationOffset;
		
		AActor* createdBox = World->SpawnActor(*StaticBoxParentClasses.Find(v.Size), &location, &rotation);
		Boxes.Add(createdBox);
	}
}


