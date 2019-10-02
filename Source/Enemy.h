// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Enemy.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemy(const class FObjectInitializer& PCIP);

	// How fast he is
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		float Speed;

	// The hitpoints the monster has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		float HP;

	// Experience gained for defeating
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		int32 Experience;

	// The type of item dropped by the monster
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		UClass* BPLoot;

	// The amount of damage attacks do, gets added to weapon damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		float BaseAttackDamage;

	// Amount of time the monster needs to rest between attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		float AttackTimeout;

	// Time since monster's last strike, readable in blueprints
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemyProperties)
		float TimeSinceLastStrike;

	// Range for his sight
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
		USphereComponent* SightSphere;

	// Range for his attack. Visualizes as a sphere in editor
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
		USphereComponent* AttackRangeSphere;

	// The bullet class the enemy uses. If this is not set, it uses a melee attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		UClass* BPBullet;

	// The amount of damage attacks do, gets added to weapon damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		float BulletLaunchImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
		bool MovingEnemy;



	bool isInAttackRange(float d)
	{
		return d < AttackRangeSphere->GetScaledSphereRadius();
	}
	bool isInSightRange(float d)
	{
		return d < SightSphere->GetScaledSphereRadius();
	}

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void Attack(AActor* thing);

	FVector Knockback;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


};