// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() { PrimaryComponentTick.bCanEverTick = true; }

// Called when the game starts
void UGrabber::BeginPlay() {
  Super::BeginPlay();
  FindPhysicsHandleComponent();
  SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // if the physics handle is attached
  // move the object that we're holding
}

void UGrabber::Grab() {
  UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"));

  /// LINE TRACE and see if we reachs any actors with physics body collision
  /// channel set
  GetFirstPhysicsBodyInReach();

  /// If we hit something then attach a physics handle
  // TODO attach physics handle
}
void UGrabber::Release() {
  UE_LOG(LogTemp, Warning, TEXT("Grab released!"));
  // TODO release physics handle
}

void UGrabber::FindPhysicsHandleComponent() {
  PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

  if (PhysicsHandle) {
  } else {
    UE_LOG(LogTemp, Error, TEXT("%s missing physic handler component!"),
           *GetOwner()->GetName());
  }
}

/// Look for attached input component (only appears at run time)
void UGrabber::SetupInputComponent() {
  // Look for input component
  InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
  if (InputComponent) {
    UE_LOG(LogTemp, Warning, TEXT("Input component found!"));
    // Bind the input axis
    InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
  } else {
    UE_LOG(LogTemp, Error, TEXT("%s missing input component!"),
           *GetOwner()->GetName());
  }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
  /// Get Player view point this tick
  FVector PlayViewPointLocation;
  FRotator PlayViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
      OUT PlayViewPointLocation, OUT PlayViewPointRotation);

  FVector LineTraceEnd =
      PlayViewPointLocation + React * PlayViewPointRotation.Vector();

  // See what we hit
  // Setup Query Params
  FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
  // Line-trace AKA ray-csast out to reach distance
  FHitResult Hit;
  GetWorld()->LineTraceSingleByObjectType(
      OUT Hit, PlayViewPointLocation, LineTraceEnd,
      FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
      TraceParameters);

  AActor *ActorHit = Hit.GetActor();
  if (ActorHit) {
    UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName());
  }
  return FHitResult();
}
