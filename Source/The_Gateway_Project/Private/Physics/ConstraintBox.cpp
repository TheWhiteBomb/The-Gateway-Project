// Fill out your copyright notice in the Description page of Project Settings.


#include "Physics/ConstraintBox.h"

// Sets default values
AConstraintBox::AConstraintBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConstraintBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConstraintBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

