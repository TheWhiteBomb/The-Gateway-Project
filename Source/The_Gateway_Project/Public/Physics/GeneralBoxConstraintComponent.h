// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "AttachmentRelation.h"
#include "Components/SphereComponent.h"

#include "GeneralBoxConstraintComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THE_GATEWAY_PROJECT_API UGeneralBoxConstraintComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	UGeneralBoxConstraintComponent();

	/// Attaches two GeneralBoxConstraintComponents from different Actors.
	/// @param constraint 
	UFUNCTION()
	void MakeAttachment(UGeneralBoxConstraintComponent* constraint);

private:
	/// Marks the 2nd GeneralBoxConstraintComponent to track the 'other' one so that
	/// it knows what it's attached to, even if the self PhysicsConstraint isn't the
	/// one that is attached.
	/// @param other
	UFUNCTION()
	void UpdateAttachmentFromOther(UGeneralBoxConstraintComponent* other);

	/// Checks whether two GeneralBoxConstraintComponents can attach.
	/// @return The reason the components can't attach (otherwise unset.)
	UFUNCTION()
	static TOptional<FString> ComponentsCanAttach(UGeneralBoxConstraintComponent* a, UGeneralBoxConstraintComponent* b);
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere)
	EAttachmentRelation ConstraintHandler = EAttachmentRelation::Unlinked;

	UPROPERTY(EditAnywhere)
	TOptional<UGeneralBoxConstraintComponent*> Relation;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	TOptional<UGeneralBoxConstraintComponent*> GetRelation() const;

	UFUNCTION(BlueprintCallable)
	TOptional<EAttachmentRelation> GetConstraintHandler() const;
};
