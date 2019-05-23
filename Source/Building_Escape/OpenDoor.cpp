// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
    // Set this component to be initialized when the game starts, and to be
    // ticked every frame.  You can turn these features off to improve
    // performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
    Super::BeginPlay();

    Owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Poll the trigger volume
    /// TODO make into a parameter
    if (GetTotalMassOfActorsOnPlate() > 50.f) {
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }

    // Close door
    if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) >= DoorCloseDelay) {
        CloseDoor();
    }
}

void UOpenDoor::OpenDoor() {
    Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor() {
    Owner->SetActorRotation(FRotator(0.0f, CloseAngle, 0.0f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const {
    float TotalMass = 0.0f;

    // Find all the overlapping actors
    TArray<AActor *> OverLappingActors;
    PressurePlate->GetOverlappingActors(OUT OverLappingActors);

    // Iterate throught add mass
    for (const auto &OverLappingActor : OverLappingActors) {
        TotalMass +=
            OverLappingActor->FindComponentByClass<UPrimitiveComponent>()
                ->GetMass();
    }

    UE_LOG(LogTemp, Warning, TEXT("Total mass %f"), TotalMass);
    return TotalMass;
}