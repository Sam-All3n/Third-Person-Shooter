// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "ThirdPersonShooter.h"
#include "PickupItem.h"
#include "HUD1.h"
#include "Bullet.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAvatar::AAvatar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();

	crouching = false;
	jumping = false;
	Hp = 100.0f;
	MaxHp = 100.0f;
	ammo = 20;
	EnemiesRemaining = 4;
}

// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// apply knockback vector
	AddMovementInput(Knockback, 1.f);

	// half it each frame
	Knockback *= 0.5f;

	if (EnemiesRemaining <= 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAvatar::ResetGame, timerTime, true);
	}

}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &AAvatar::Forward);
	PlayerInputComponent->BindAxis("Backward", this, &AAvatar::Backward);
	PlayerInputComponent->BindAxis("Right", this, &AAvatar::Right);
	PlayerInputComponent->BindAxis("Left", this, &AAvatar::Left);
	PlayerInputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);
	PlayerInputComponent->BindAction("doCrouch", IE_Pressed, this, &AAvatar::PlayerCrouch);
	PlayerInputComponent->BindAction("doJump", IE_Pressed, this, &AAvatar::PlayerJump);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AAvatar::ToggleInventory);
	PlayerInputComponent->BindAction("MouseClickedLMB", IE_Pressed, this, &AAvatar::MouseClicked);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AAvatar::Shoot);




}

void AAvatar::Forward(float amount)
{
	if (inventoryShowing)
	{
		return;
	}

	// Don't enter the body of this function if Controller is
	// not set up yet, or if the amount to move is equal to 0
	if (Controller && amount)
	{
		FVector fwd = GetActorForwardVector();
		// we call AddMovementInput to actually move the player
		// by `amount` in the `fwd` direction
		AddMovementInput(fwd, amount);
	}


}

void AAvatar::Backward(float amount)
{
	if (inventoryShowing)
	{
		return;
	}

	// Don't enter the body of this function if Controller is
	// not set up yet, or if the amount to move is equal to 0
	if (Controller && amount)
	{
		FVector bkwrd = GetActorForwardVector() * -1;
		// we call AddMovementInput to actually move the player
		// by `amount` in the `fwd` direction
		AddMovementInput(bkwrd, amount);
	}


}


void AAvatar::Right(float amount)
{
	if (inventoryShowing)
	{
		return;
	}

	if (Controller && amount)
	{
		FVector right = GetActorRightVector();
		AddMovementInput(right, amount);
	}
}

void AAvatar::Left(float amount)
{
	if (inventoryShowing)
	{
		return;
	}

	if (Controller && amount)
	{
		FVector left = GetActorRightVector() * -1;
		AddMovementInput(left, amount);
	}

}

void AAvatar::Yaw(float amount)
{
	if (inventoryShowing)
	{
		// if the button is down click drag
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AHUD1* hud = Cast<AHUD1>(PController->GetHUD());
		hud->MouseMoved();


		return;
	}

	AddControllerYawInput(150.f * amount * GetWorld()->GetDeltaSeconds());

}

void AAvatar::Pitch(float amount)
{
	if (inventoryShowing)
	{
		// if the button is down click drag
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AHUD1* hud = Cast<AHUD1>(PController->GetHUD());
		hud->MouseMoved();
		return;
	}

	AddControllerPitchInput(150.f * amount * GetWorld()->GetDeltaSeconds());

}

void AAvatar::PlayerCrouch()
{
	if (inventoryShowing)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerCrouch() -- CanCrouch() = %d"), CanCrouch());
	if (crouching)
	{
		UnCrouch();
		crouching = false;
	}
	else
	{
		Crouch();
		crouching = true;
	}

}

void AAvatar::PlayerJump()
{
	if (inventoryShowing)
	{
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("PlayerJump() -- jumping = %d"), jumping);
	if (jumping)
	{
		StopJumping();
		jumping = false;
	}
	else
	{
		jumping = true;
		Jump();
		jumping = false;

	}

}

void AAvatar::Pickup(APickupItem *item)
{
	if (Backpack.Contains(item->Name))
	{
		Backpack[item->Name] += item->Quantity;
	}
	else
	{
		Backpack.Add(item->Name, item->Quantity);
		Icons.Add(item->Name, item->Icon);
	}
}

void AAvatar::ToggleInventory()
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	AHUD1* hud = Cast<AHUD1>(PController->GetHUD());
	if (inventoryShowing)
	{
		hud->clearWidgets();
		inventoryShowing = false;
		PController->bShowMouseCursor = false;
		return;
	}
	else
	{
		inventoryShowing = true;
		PController->bShowMouseCursor = true;
		for (TMap<FString, int>::TIterator it = Backpack.CreateIterator(); it; ++it)
		{
			// Combine string name of the item, with qty eg Cow x 5
			FString fs = it->Key + FString::Printf(TEXT(" x %d"), it->Value);
			UTexture2D* tex = NULL;
			if (Icons.Find(it->Key))
			{
				tex = Icons[it->Key];
				Widget w(Icon(fs, tex), NULL);
				hud->addWidget(w);
			}
		}
	}

}


void AAvatar::MouseClicked()
{
	if (inventoryShowing)
	{
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AHUD1* hud = Cast<AHUD1>(PController->GetHUD());
		hud->MouseClicked();
	}
}


float AAvatar::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Hp -= Damage;
	// add some knockback that gets applied over a few frames
	Knockback = GetActorLocation() - DamageCauser->GetActorLocation();
	Knockback.Normalize();
	Knockback *= Damage * 500;

	// if hp goes below 0 hp, he will die
	if (Hp <= 0)
	{
		Hp = 0; //clamp
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAvatar::ResetGame, timerTime, true);
	}
	return Damage;
}

void AAvatar::Shoot()
{
	ammo = ammo - 1;

	if(ammo < 0)
	{
		ammo = 0;
	}

	if (ammo > 0)
	{

		if (BPBullet)
		{
			FVector fwd = GetActorForwardVector();
			FVector nozzle = GetMesh()->GetBoneLocation("index_03_l");
			// move it fwd of the monster so it doesn't
			// collide with the monster model.
			nozzle += fwd * 70;

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
}

void AAvatar::ResetGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
