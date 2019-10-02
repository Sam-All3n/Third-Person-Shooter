// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "ThirdPersonShooter.h"


// Sets default values
ABullet::ABullet(const class FObjectInitializer& PCIP)
{

	PrimaryActorTick.bCanEverTick = true;

	// We want the bullet to simulate physics using
	// its bounding sphere.
	ProxSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProxSphere"));
	RootComponent = ProxSphere;
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::Prox);

	// For the bullet, the mesh is "dumb". It just "tags along"
	// with whatever the ProxSphere is doing, since the ProxSphere
	// is going to respond to collisions with objects.
	// Attach the Mesh to the ProxSphere.
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(ProxSphere);
	Damage = 10;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Prox_Implementation(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp != OtherActor->GetRootComponent())
	{
		// don't collide w/ anything other than the actor's root component
		return;
	}
	OtherActor->TakeDamage(Damage, FDamageEvent(), NULL, this);
	Destroy();
}


