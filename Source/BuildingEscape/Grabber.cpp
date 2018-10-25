// Copyright Euclides Araujo 2018.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if( PhysicsHandle == nullptr)
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing PhysicsHandle component!" ), *GetOwner()->GetName() )
	}
}

/// Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if( InputComponent )
	{
		InputComponent->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
		InputComponent->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing Input component!" ), *GetOwner()->GetName() )
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if( !PhysicsHandle ) { return; }
	// If the physics handle is attached
	if( PhysicsHandle->GrabbedComponent )
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation( GetLineTracePoints().v2 );
	}
}

void UGrabber::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();	// gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if( ActorHit != nullptr )
	{
		if( !PhysicsHandle ) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,	// No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()	// keeps the same rotation
		);
	}
}

void UGrabber::Release()
{
	if( !PhysicsHandle ) { return; }
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );
	FTwoVectors TracePoints = GetLineTracePoints();
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		TracePoints.v1,
		TracePoints.v2,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		TraceParameters
	);

	return HitResult;
}

FTwoVectors UGrabber::GetLineTracePoints() const
{
	FVector StartLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT StartLocation,
		OUT PlayerViewPointRotation
	);
	FVector EndLocation = StartLocation + PlayerViewPointRotation.Vector() * Reach;
	return FTwoVectors( StartLocation, EndLocation );
}