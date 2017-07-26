// Copyright gendi93

#pragma once

#include "CoreMinimal.h"
#include "Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far ahead of the player we can reach
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-cast and grab the object in reach
	void Grab();
	// Called when grab is released
	void Release();
	// Look for attached physics handle
	void FindPhysicsHandleComponent();
	// Look for attached input component
	void SetupInputComponent();
	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	// Return current start and end of reach line
	TArray<FVector> GetView();
};
