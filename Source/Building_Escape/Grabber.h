// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UGrabber : public UActorComponent {
    GENERATED_BODY()

  private:
    float React = 100.f;

    UPhysicsHandleComponent *PhysicsHandle = nullptr;

    UInputComponent *InputComponent = nullptr;

    // Ray-cast and grab what's in reach
    void Grab();

    // Grab is released
    void Release();

    // Find (assumed) attached physics handler
    void FindPhysicsHandleComponent();

    // Setup (assumed) attached input component
    void SetupInputComponent();

    // return hit for first physics body in reach
    const FHitResult GetFirstPhysicsBodyInReach();

  public:
    // Sets default values for this component's properties
    UGrabber();

  protected:
    // Called when the game starts
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void
    TickComponent(float DeltaTime, ELevelTick TickType,
                  FActorComponentTickFunction *ThisTickFunction) override;
};
