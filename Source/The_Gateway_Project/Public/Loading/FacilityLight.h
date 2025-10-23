// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FacilityLight.generated.h"

UCLASS()
class THE_GATEWAY_PROJECT_API AFacilityLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFacilityLight();

	UFUNCTION(BlueprintCallable, Category = "Visibility")
	void SetEnabled(const bool Enabled) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Visibility")
	TArray<UMaterial*> DisabledMaterials;

	UPROPERTY(EditAnywhere, Category="Visibility")
	TArray<UMaterial*> EnabledMaterials;
	
	UPROPERTY(EditAnywhere, Category="Visibility")
	UStaticMesh* Mesh;
	
private:
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;
};
