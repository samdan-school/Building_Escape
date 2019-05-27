// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent {
	GENERATED_BODY()

private:
	float OpenAngle = 0.0f;
	float CloseAngle = 90.0f;
	float LastDoorOpenTime = 0.f;
	AActor *Owner = nullptr;

	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(BluePrintAssignable)
		FOnOpenRequest OnOpenRequest;

	UPROPERTY(BluePrintAssignable)
		FOnCloseRequest OnCloseRequest;

	// return total mass in kg
	float GetTotalMassOfActorsOnPlate() const;

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
	virtual void
		TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
