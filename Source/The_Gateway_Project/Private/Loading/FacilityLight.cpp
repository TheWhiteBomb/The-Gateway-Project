// Fill out your copyright notice in the Description page of Project Settings.


#include "Loading/FacilityLight.h"

#include "Components/LightComponentBase.h"

// Sets default values
AFacilityLight::AFacilityLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);
}

void AFacilityLight::OnConstruction(const FTransform& Transform)
{
	MeshComponent->SetStaticMesh(Mesh);
	for (int i = 0; i < Materials.Num(); i++)
	{
		MeshComponent->SetMaterial(i, Materials[i]);
	}
}

// Called when the game starts or when spawned
void AFacilityLight::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFacilityLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFacilityLight::SetEnabled(const bool Enabled) const
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	for (int i = 0; i < Components.Num(); i++)
	{
		UActorComponent* v = Components[i];
		if (v->IsA<ULightComponentBase>())
		{
			ULightComponentBase* Light = Cast<ULightComponentBase>(v);
			Light->SetVisibility(Enabled);
		}
	}
}