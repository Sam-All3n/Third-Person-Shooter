// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingActor2.h"


// Sets default values
AFloatingActor2::AFloatingActor2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloatingActor2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloatingActor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * 100.0f; //Scale our height by a factor of 20
	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);
}

