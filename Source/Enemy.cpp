// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "ThirdPersonShooter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Bullet.h"
#include "Avatar.h"


// Sets default values

AEnemy::AEnemy(const class FObjectInitializer& PCIP) : Super(PCIP)
{

	PrimaryActorTick.bCanEverTick = true;
	Speed = 100;
	HP = 30;
	Experience = 0;
	BPLoot = NULL;
	AttackTimeout = 1.5f;
	TimeSinceLastStrike = 0;
	SightSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SightSphere"));
	SightSphere->SetupAttachment(RootComponent);
	AttackRangeSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	
	
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	
	// move the monster towards the player
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar) return;


	FVector playerPos = avatar->GetActorLocation();
	FVector toPlayer = playerPos - GetActorLocation();
	float distanceToPlayer = toPlayer.Size();
	

	//Second Step of Tutorial
	// If the player is not the SightSphere of the monster, go back
	if (distanceToPlayer > SightSphere->GetScaledSphereRadius())
	{
		// If the player is OS, then the enemy cannot chase
		return;
	}

	//First Step of Tutorial
	toPlayer /= distanceToPlayer; // normalizes the vector

	//Commented out as we don't want the player to move this time
	
	
	
    
	
	// At least face the target
	// Gets you the rotator to turn something
	// that looks in the `toPlayer` direction.
	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0; // 0 off the pitch
	RootComponent->SetWorldRotation(toPlayerRotation);

	if (isInAttackRange(distanceToPlayer))
	{
		// Perform the attack.
		if (!TimeSinceLastStrike)
		{
			// If the cooldown is over, then attack again
			// This resets the hit parameter.
			Attack(avatar);
		}
		TimeSinceLastStrike += DeltaTime;

		if (TimeSinceLastStrike > AttackTimeout)
		{
			TimeSinceLastStrike = 0;
		}
		return; // nothing else to do.
	}

	if (MovingEnemy == true)
	{
		AddMovementInput(toPlayer, Speed*DeltaTime);

	}
}


void AEnemy::Attack(AActor* thing)
{
	if (BPBullet)
	{
		FVector fwd = GetActorForwardVector();
		FVector nozzle = GetMesh()->GetBoneLocation("index_03_l");
		// move it fwd of the monster so it doesn't
		// collide with the monster model.
		nozzle += fwd * 70;
		FVector toOpponent = thing->GetActorLocation() - nozzle;
		toOpponent.Normalize();
		ABullet *bullet = GetWorld()->SpawnActor<ABullet>(BPBullet, nozzle, RootComponent->GetComponentRotation());

		if (bullet)
		{
			bullet->Firer = this;
			bullet->ProxSphere->AddImpulse(fwd*BulletLaunchImpulse);
			//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "Enemy: Bullet Spawned");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "Enemy: no bullet actor could be spawned.is the bullet overlapping something ? ");
		}
	}
}



// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}


float AEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	HP -= Damage;
	// add some knockback that gets applied over a few frames
	Knockback = GetActorLocation() - DamageCauser->GetActorLocation();
	Knockback.Normalize();
	Knockback *= Damage * 500;
	// if hp goes below 0 hp, he will die
	
	if (HP <= 0)
	{		
		AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		avatar -> EnemiesRemaining--;
		Destroy(this);
		
	}
	return Damage;
}


