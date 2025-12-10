// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralBoxConstraintComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ConstraintBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstraintAny);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBrokenAny);

UCLASS()
class THE_GATEWAY_PROJECT_API AConstraintBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AConstraintBox();

	UFUNCTION(BlueprintCallable)
	void UpdateCascade();

	UFUNCTION(BlueprintCallable)
	void SetCascadeIsHeld(bool newIsHeld);
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void CheckConstraints() const;

	UFUNCTION()
	void CheckSingleConstraint(UGeneralBoxConstraintComponent* constraint) const;

	UFUNCTION()
	void CascadeSingleUpdate(AConstraintBox* dominant);
	
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnConstraintAny OnConstraintAny;

	UPROPERTY(BlueprintAssignable)
	FOnBrokenAny OnBrokenAny;

	UPROPERTY(BlueprintReadOnly)
	TArray<AConstraintBox*> CascadeBoxes;

	UPROPERTY(BlueprintReadOnly)
	TArray<UGeneralBoxConstraintComponent*> CascadeConstraints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttachmentSphereTraceRadius = 32.0f;
	
	UPROPERTY(BlueprintReadOnly)
	bool IsHeld = false;
};
