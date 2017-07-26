// Copyright gendi93

#include "Grabber.h"
#include <Engine/World.h>
#include <GameFramework/PlayerController.h>
#include <DrawDebugHelpers.h>
#include <Components/PrimitiveComponent.h>

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

	if (!PhysicsHandle)
	{
		return;
	}
	// If the physics handle is attached, move the object that we're holding
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetView()[1]);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}
}
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PhysicsHandle)
	{
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
	// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // Gets the mesh in our case
	auto ActorHit = HitResult.GetActor();
	// If we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)
		{
			return;
		}
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allow rotation
		);
	}
}
void UGrabber::Release()
{
	if (!PhysicsHandle)
	{
		return;
	}
	PhysicsHandle->ReleaseComponent();
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetView()[0],
		GetView()[1],
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}
TArray<FVector> UGrabber::GetView() {
	TArray<FVector> View;
	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	View.Add(PlayerViewPointLocation);
	View.Add(LineTraceEnd);
	return View;
}

