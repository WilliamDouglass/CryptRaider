// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();	
	// ...
	// UE_LOG(LogTemp, Display, TEXT("Trigger Alive"));
	
}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Actor = GetKeyActor();
	if (Actor != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor Detected: %s"), *Actor->GetActorNameOrLabel());
		if(triggerEnter) // snaps actor to snap location
		{
			Actor->SetActorLocation(SnapLocation);
			FRotator CurRotation = Actor->GetActorRotation();
			CurRotation.Pitch = 0.0f;  // Set Pitch (X) to 0
			CurRotation.Roll = 0.0f; 
			Actor->SetActorRotation(CurRotation);
			triggerEnter = false;
		}

		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		
		Actor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);

		// UE_LOG(LogTemp, Display, TEXT("Actor: %s Detected || Mover Status: %s"),*Actor->GetActorNameOrLabel(), Mover ? TEXT("Not Null") : TEXT("Null"));
		if(Mover)
		{
			Mover->SetShouldMove(true);
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover Invalid"));
		}
	}else
	{
		triggerEnter = true;
		if(Mover)
		{
			Mover->SetShouldMove(false);
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover Invalid"));
		}
	}

}


AActor* UTriggerComponent::GetKeyActor() const
{
	TArray<AActor*> ActorArr;
	GetOverlappingActors(ActorArr);
	for (AActor* Actor : ActorArr)
	{
		if (Actor->ActorHasTag(UnlockTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}


