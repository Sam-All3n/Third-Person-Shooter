// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RotatableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatableMesh"));

	AnimTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AnimationTrigger"));
	AnimTrigger->SetGenerateOverlapEvents(true);
	AnimTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnAnimTriggered);
	AnimTrigger->AttachToComponent(RotatableMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}


// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	if (AnimCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleProgress"));
		MyTimeline.AddInterpFloat(AnimCurve, ProgressFunction);
		MyTimeline.SetLooping(false);
		InitialRotation = TargetRotation = GetActorRotation();
		if (RotationAxis == "Yaw")
		{
			TargetRotation.Yaw += MaxRotation;
		}
		else if (RotationAxis == "Pitch")
		{
			TargetRotation.Pitch += MaxRotation;
		}
		else if (RotationAxis == "Roll")
		{
			TargetRotation.Roll += MaxRotation;
		}
	}
}


// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyTimeline.TickTimeline(DeltaTime);
}


void ADoor::HandleProgress(float Value)
{
	FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, Value);
	SetActorRotation(NewRotation);
}


void ADoor::OnAnimTriggered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!MyTimeline.IsPlaying())
	{
		MyTimeline.PlayFromStart();
	}
}
