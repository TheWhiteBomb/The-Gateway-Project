// Fill out your copyright notice in the Description page of Project Settings.


#include "Physics/GeneralBoxConstraintComponent.h"

#include "GatewayConfiguration.h"
#include "MovieSceneTracksComponentTypes.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values for this component's properties
UGeneralBoxConstraintComponent::UGeneralBoxConstraintComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGeneralBoxConstraintComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UGeneralBoxConstraintComponent::MakeAttachment(UGeneralBoxConstraintComponent* constraint)
{
	auto canAttach = ComponentsCanAttach(this, constraint);
	if (canAttach.IsSet())
	{
		auto reason = canAttach.Get(TEXT("UNKNOWN REASON!"));
		UE_LOG(GWLogBoxConstraint, Log, TEXT("Attempted to make attachment but %s"), *reason);
		return;
	}
	
	ConstraintHandler = EAttachmentRelation::Self;
	constraint->UpdateAttachmentFromOther(this);
	Relation.Emplace(constraint);

	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Green, TEXT("Constraint attaching."));
	constraint->GetOwner()->SetActorLocation(FVector(0, 0, 0));
	auto test = constraint->GetOwner()->K2_AttachToActor(GetOwner(), NAME_None, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	//auto test = constraint->GetOwner()->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	UE_LOG(GWLogBoxConstraint, Log, TEXT("Attachment working: %s."), test ? TEXT("WORKING") : TEXT("NOT WOKING"));
}

void UGeneralBoxConstraintComponent::UpdateAttachmentFromOther(UGeneralBoxConstraintComponent* other)
{
	ConstraintHandler = EAttachmentRelation::Other;
	Relation.Emplace(other);
}

TOptional<FString> UGeneralBoxConstraintComponent::ComponentsCanAttach(UGeneralBoxConstraintComponent* a, UGeneralBoxConstraintComponent* b)
{
	if (a->GetOwner() == b->GetOwner())
		return TOptional<FString>(TEXT("GeneralBoxConstraintComponents belong to the same actor; ignoring."));

	if (a->ConstraintHandler != EAttachmentRelation::Unlinked)
		return TOptional<FString>(TEXT("GeneralBoxConstraintComponent (SELF) is not unlinked; ignoring."));

	if (b->ConstraintHandler != EAttachmentRelation::Unlinked)
		return TOptional<FString>(TEXT("GeneralBoxConstraintComponent (OTHER) is not unlinked; ignoring.")); 

	return TOptional<FString>();
}


void UGeneralBoxConstraintComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGeneralBoxConstraintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TOptional<UGeneralBoxConstraintComponent*> UGeneralBoxConstraintComponent::GetRelation() const
{
	return Relation;
}

TOptional<EAttachmentRelation> UGeneralBoxConstraintComponent::GetConstraintHandler() const
{
	return ConstraintHandler;
}