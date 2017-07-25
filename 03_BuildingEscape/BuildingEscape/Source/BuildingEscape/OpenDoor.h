// Copyright gendi93

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();

	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Door angle
	UPROPERTY(EditAnywhere)
	float OpenAngle = 50.0f;

	UPROPERTY(EditAnywhere)
	float CloseAngle = -90.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
	bool DoorOpen;

	float LastDoorOpenTime;
	float DoorCloseTime;

	// Pressure plate trigger
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	// Object to be detected by pressure plate
	AActor* ActorThatOpens;
	// The owning door
	AActor* Owner;
		
	
};
