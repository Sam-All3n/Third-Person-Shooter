// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API ADoor : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ADoor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Axis to rotate on
	UPROPERTY(EditAnywhere)
		FString RotationAxis = "Yaw";

	// Static mesh for rotatable object
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* RotatableMesh;

	// Box trigger to start the animation
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* AnimTrigger;

	// Maximum spin that the object will reach
	UPROPERTY(EditAnywhere, Category = "Animation")
		float MaxRotation;

	// Pointer to CurveFloat instance
	UPROPERTY(EditAnywhere, Category = "Animation")
		UCurveFloat* AnimCurve;
private:

	// Initial rotation
	FRotator InitialRotation;

	// Target rotation
	FRotator TargetRotation;

	// Timeline to spin
	FTimeline MyTimeline;

	UFUNCTION()
		void HandleProgress(float Value);

	UFUNCTION()
		void OnAnimTriggered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
