// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

class APickupItem;

UCLASS()
class THIRDPERSONSHOOTER_API AAvatar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAvatar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//New member function declarations
	//These will be used to move the player around
	void Forward(float amount);
	void Backward(float amount);
	void Left(float amount);
	void Right(float amount);
	void Yaw(float amount);
	void Pitch(float amount);
	void PlayerCrouch();
	void PlayerJump();
	void Shoot();
	void ToggleInventory();
	void MouseClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerProperties)
		float Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerProperties)
		float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerProperties)
		int ammo;

	// The bullet class the enemy uses. If this is not set, it uses a melee attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerProperties)
		UClass* BPBullet;

	// The amount of damage attacks do, gets added to weapon damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerProperties)
		float BulletLaunchImpulse;

	// The amount of damage attacks do, gets added to weapon damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerProperties)
		float BaseAttackDamage;

	bool crouching;
	bool jumping;

	TMap<FString, int> Backpack;

	TMap<FString, UTexture2D*> Icons;

	bool inventoryShowing;

	void Pickup(APickupItem *item);

	FVector Knockback;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	

	UAnimSequence *Anim;

	int EnemiesRemaining;

	void ResetGame();

	float timerTime = 3.0f;
	


};
