// Fill out your copyright notice in the Description page of Project Settings.

#include "Loading/BoxDeliveryProgressController.h"
#include "Kismet/GameplayStatics.h"

#include "GatewayConfiguration.h"
#include "Loading/FacilityLight.h"

void ABoxDeliveryProgressController::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);
	
	int lightCount = FacilityLights.Num();
	for (int i = 0; i < lightCount; i++)
	{
		auto v = FacilityLights[i];
		auto pointLight = Cast<AFacilityLight>(v);
			
		float transitionPhase = static_cast<float>(i)/static_cast<float>(lightCount);
		
		if (TransitionProgress < 0.5f)
		{
			float subTransitionProgress = TransitionProgress*2;
			pointLight->SetEnabled(subTransitionProgress < transitionPhase);
		}
		else
		{
			float undoTransitionPhase = 1-transitionPhase;

			float subTransitionProgress = (TransitionProgress-0.5f)*2;
			pointLight->SetEnabled(subTransitionProgress > undoTransitionPhase);
		}
	}
	
	if (TransitionProgress >= 0.5f) { FinalizeSequence(); }
}

void ABoxDeliveryProgressController::OnPreTransition()
{
	TransitionDuration = 3;
	FacilityLights = GetFacilityLights();
	UE_LOG(GWLogDayChange, Display, TEXT("PointLights found: %d"), FacilityLights.Num())
}

void ABoxDeliveryProgressController::OnDestroyAnyReason()
{
	for (auto actor : FacilityLights)
	{
		AFacilityLight* v = Cast<AFacilityLight>(actor);
		v->SetEnabled(true);
	}
}


TArray<AActor*> ABoxDeliveryProgressController::GetFacilityLights() const
{
	TArray<AActor*> lights;
	UGameplayStatics::GetAllActorsOfClass(this, AFacilityLight::StaticClass(), lights);

	UE_LOG(GWLogDebug, Display, TEXT("Found facility light anywhere"));

	
	return lights;
}

