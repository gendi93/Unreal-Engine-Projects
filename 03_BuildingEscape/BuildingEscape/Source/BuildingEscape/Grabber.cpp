// Copyright gendi93

#include "Grabber.h"
#include <Engine/World.h>
#include <GameFramework/PlayerController.h>
#include <DrawDebugHelpers.h>
#include <Runtime/Engine/Classes/Components/PrimitiveComponent.h>

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

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation
		+ PlayerViewPointRotation.Vector()*Reach;

	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	// move the object that we're holding
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("%s's physics handle component found"), *(GetOwner()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}
}
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("%s's input component found"), *(GetOwner()->GetName()));
		// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));
	}
}
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grab pressed"));
	// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		// If we hit something then attach a physics handle
		// Attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}
void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Grab released"));
	PhysicsHandle->ReleaseComponent();
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation
		+ PlayerViewPointRotation.Vector()*Reach;
	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has been hit"), *(ActorHit->GetName()));
	}

	return Hit;
}

