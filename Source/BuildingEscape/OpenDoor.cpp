// Copyright Euclides Araujo 2018.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	FString ObjectName = Owner->GetName();
	FString ObjectRotation = Owner->GetTransform().GetRotation().ToString();
	//FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToString();
	UE_LOG( LogTemp, Warning, TEXT( "%s rotation is %s" ), *ObjectName, *ObjectRotation );
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

