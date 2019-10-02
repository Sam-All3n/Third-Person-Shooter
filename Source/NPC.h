// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Avatar.h"
#include"HUD1.h"
#include "NPC.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API ANPC : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ANPC(const FObjectInitializer& PCIP);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent*PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision) USphereComponent* ProxSphere;

	//The function InProximity to create the new collider sphere
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void InProximity(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// This is the NPC's message that he has to tell us.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPCMessage)
		FString NpcMessage;

	// When you create a blueprint from this class, you want to be able to edit that message in blueprints that's why we have the EditAnywhere and BlueprintReadWrite properties.
	// This is the NPC's name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPCMessage) 
		FString Name;

	////This is the property to add a 2D Texture
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPCMessage)
		UTexture2D* Face;

};