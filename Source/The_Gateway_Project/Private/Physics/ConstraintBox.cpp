// Fill out your copyright notice in the Description page of Project Settings.


#include "Physics/ConstraintBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/GeneralBoxConstraintComponent.h"

AConstraintBox::AConstraintBox()
{
	PrimaryActorTick.bCanEverTick = true;
	UpdateCascade();

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AConstraintBox::UpdateCascade()
{
	CascadeBoxes = TArray<AConstraintBox*>();
	CascadeConstraints = TArray<UGeneralBoxConstraintComponent*>();
	
	CascadeSingleUpdate(this);

	for (auto box : CascadeBoxes)
	{
		box->CascadeBoxes = CascadeBoxes;
		box->CascadeConstraints = CascadeConstraints;
	}
}

void AConstraintBox::CascadeSingleUpdate(AConstraintBox* dominant)
{
	if (dominant->CascadeBoxes.Contains(this)) { return; }
	dominant->CascadeBoxes.Add(this);
	
	for (auto component : GetComponents())
	{
		if (!component->IsA<UGeneralBoxConstraintComponent>()) { continue; }
		auto constraint = Cast<UGeneralBoxConstraintComponent>(component);

		if (dominant->CascadeConstraints.Contains(constraint)) { continue; }
		dominant->CascadeConstraints.Add(constraint);

		CascadeSingleUpdate(dominant);
	}
}

void AConstraintBox::SetCascadeIsHeld(bool newIsHeld)
{
	for (auto box : CascadeBoxes)
	{
		box->IsHeld = newIsHeld;
	}
}

void AConstraintBox::BeginPlay()
{
	Super::BeginPlay();
}

void AConstraintBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// DEBUG
	CheckConstraints();
}

void AConstraintBox::CheckConstraints() const
{
	for (auto component : GetComponents())
	{
		if (!component->IsA(UGeneralBoxConstraintComponent::StaticClass())) { continue; }
		auto constraintComponent = Cast<UGeneralBoxConstraintComponent>(component);
		
		CheckSingleConstraint(constraintComponent);
	}
}

void AConstraintBox::CheckSingleConstraint(UGeneralBoxConstraintComponent* constraint) const
{
	auto loc = constraint->GetComponentLocation();
	auto collisionQueryType = UEngineTypes::ConvertToTraceType(ECC_PhysicsBody);
	
	TArray<FHitResult> outHits;
	
	UKismetSystemLibrary::SphereTraceMulti(
		this,
		loc,
		loc,
		AttachmentSphereTraceRadius,
		collisionQueryType,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		outHits,
		true);
	
	for (auto OutHit : outHits)
	{
		auto outActor = OutHit.GetActor();
		auto outComponent = OutHit.GetComponent();
		if (outActor->IsA(StaticClass()) && outComponent->IsA(UGeneralBoxConstraintComponent::StaticClass()))
		{
			auto constraintComponent = Cast<UGeneralBoxConstraintComponent>(outComponent);
			OnConstraintAny.Broadcast();
			constraint->MakeAttachment(constraintComponent);
			
			break;
		}
	}
}



