// Copyright Euclides Araujo 2018.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

#define OUT

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
	Owner = GetOwner();

	if( PressurePlate == nullptr )
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing PressurePlate!" ), *GetOwner()->GetName() )
	}

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Pool the Trigger Volume
	if( GetTotalMassOfActorsOnPlate() > TriggerMass )
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	if( !PressurePlate ) { return TotalMass; }

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );

	// Iterate through them adding their masses

	for( AActor* Actor : OverlappingActors )
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG( LogTemp, Warning, TEXT( "%s on pressure plate, mass: " ), *Actor->GetName() )		
	}
	
	return TotalMass;
}