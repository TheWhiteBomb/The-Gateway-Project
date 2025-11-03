// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Loading/BoxDeliveryProgressController.h"

#include "GatewayConfiguration.h"

void ABoxDeliveryProgressController::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);
	
	int lightCount = PointLights.Num();
	for (int i = 0; i < lightCount; i++)
	{
		auto v = PointLights[i];
		auto pointLight = Cast<APointLight>(v);
			
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
	PointLights = GetPointLights();
	UE_LOG(GWLogDayChange, Display, TEXT("PointLights found: %d"), PointLights.Num())
}

void ABoxDeliveryProgressController::OnDestroyAnyReason()
{
	for (auto actor : PointLights)
	{
		APointLight* v = Cast<APointLight>(actor);

		if (!v->IsEnabled())
		{
			v->ToggleEnabled();
		}
	}
}


TArray<AActor*> ABoxDeliveryProgressController::GetPointLights()
{
	TArray<AActor*> pointLights;
	UGameplayStatics::GetAllActorsOfClass(this, APointLight::StaticClass(), pointLights);

	return pointLights;
}

