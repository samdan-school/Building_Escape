// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the obkect that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	// move the object that we're holding
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physic handler component!"), *GetOwner()->GetName());
	}
}

/// Look for attached input component (only appears at run time)
void UGrabber::SetupInputComponent() {
	// Look for input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		if (!PhysicsHandle) { return; }
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component!"),
			*GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	// Line-trace AKA ray-csast out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, GetReachLineStart(), GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName());
	}

	return HitResult;
}

void UGrabber::Grab() {
	/// LINE TRACE and see if we reachs any actors with physics body collision
	/// channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit != nullptr) {
		// TODO attach physics 
		PhysicsHandle->GrabComponent(
			ComponentToGrab, NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}
void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released!2"));
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineStart() const {
	/// Get Player view point this tick
	FVector PlayViewPointLocation;
	FRotator PlayViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayViewPointLocation, 
		OUT PlayViewPointRotation);

	return PlayViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const {
	/// Get Player view point this tick
	FVector PlayViewPointLocation;
	FRotator PlayViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayViewPointLocation, 
		OUT PlayViewPointRotation);

	return PlayViewPointLocation + React * PlayViewPointRotation.Vector();
}
