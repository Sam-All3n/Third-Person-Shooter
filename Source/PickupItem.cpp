// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonShooter.h"
#include "Avatar.h"
#include "HUD1.h"


// Sets default values
APickupItem::APickupItem(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	Name = "UKNOWN ITEM";
	Quantity = 0;
	PrimaryActorTick.bCanEverTick = true;
	ProxSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProxSphere"));
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::Prox);
	ProxSphere->SetupAttachment(Mesh);
}


void APickupItem::Prox_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<AAvatar>(OtherActor) == nullptr)
	{
		return;
	}
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (isAmmo)
	{
		avatar->ammo += 20;
	}

	if (isHealth)
	{
		avatar->Hp += 20;
	}
	AHUD1* hud = Cast<AHUD1>(PController->GetHUD());
	hud->addMessage(Message(Icon, FString("Picked up ")
		+ FString::FromInt(Quantity) + FString(" ") + Name, 5.f,
		FColor::White));
	// haven’t defined this method yet here, so compiling will fail. We define it later on
	avatar->Pickup(this);
	Destroy();
}



// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

