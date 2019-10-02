// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PickupItem.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API APickupItem : public AActor
{
	GENERATED_BODY()

public:
	APickupItem(const class FObjectInitializer& PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 Quantity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
		USphereComponent *ProxSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
		UStaticMeshComponent *Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool isAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool isHealth;



	UFUNCTION(BlueprintNativeEvent, Category = Colision)

	void Prox(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};

