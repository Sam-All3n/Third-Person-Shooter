// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC.h"
#include "ThirdPersonShooter.h"
#include "HUD1.h"
#include "Avatar.h"


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Sets default values
ANPC::ANPC(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	// Set this character to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProxSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProxSphere"));
	// You must attach the ProxSphere to the root component, otherwise,
	// the ProxSphere will NOT be with the NPC!
	ProxSphere->SetupAttachment(RootComponent);
	ProxSphere->SetSphereRadius(32.f);
	// Code to make ANPC::InProximity() run when this proximity sphere
	// overlaps another actor.
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::InProximity);
	NpcMessage = "Hi, I'm a Solider";//default message, can be overridden in blueprints!
}

void ANPC::InProximity_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	//this is where the code on “what to do when a collision is detected” goes.
	GLog->Log("Begin overlap has fired");
	// if the overlapped actor is the player, display my message
	AAvatar *player = Cast<AAvatar>(OtherActor);
	if (player == nullptr)
	{
		// The intersected actor wasn't the player, so ignore this overlap
		return;
	}
	// The player has multiple components inside him. we don't want an overlap event
	// for EACH component, so just check for intersection with the root component.
	if (OtherComp != player->GetRootComponent())
	{
		return;
	}
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	AHUD1 * hud = Cast<AHUD1>(PController->GetHUD());
	hud->addMessage(Message(Face, NpcMessage, 5.0f, FColor::White));
}


