// CopyRight. Asraf Noor. 2022

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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
	
	FindPhysicsHandle();
	
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}	
}

void UGrabber::FindPhysicsHandle()
{
	// Check for Physics HAndle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// nullptr is used to avoid crash when physics handle is not found and returns a nullptr from Grab
	// if (PhysicsHandle == nullptr)
	if (!PhysicsHandle)
	{	
		UE_LOG(LogTemp, Error, TEXT("No physics handle componenrt found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	// UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!")); DEBUG LINE

	// Raycast only when the key is pressed and see if we reach any actors with physics body collsion channel set.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// If we hit something then attach the physics handle.
	if (ActorHit)
	{	
		if (!PhysicsHandle) {return;}
		// TODO attach physics handle
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach()
		);
	}
}


void UGrabber::Release()
{
	if (!PhysicsHandle) {return;}
	// UE_LOG(LogTemp, Warning, TEXT("Grabber released!")); DEBUG LINE
	// remove/release physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {return;}
	// If the physics handle is attached.
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	
	// Logging out to test for Location and Rotation of Player
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
	// *PlayerViewPointLocation.ToString(),
	// *PlayerViewPointRotation.ToString()
	// );

	// Draw a line from player showing the reach DEBUG LINE. SWITCH ON/OFF
	// DrawDebugLine(
	// 	GetWorld(),
	// 	GetPlayersWorldPos(),
	// 	GetPlayerReach(),
	// 	FColor (0, 255, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	3.f
	// );

	// RAY CAST out to certain distance (Reach)
	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// // See what it hits
	// AActor* ActorHit =  Hit.GetActor();

	// if (ActorHit)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()))
	// }

	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	// Get PlayerViewpont
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;

}

FVector UGrabber::GetPlayerReach() const
{	// Get PlayerViewpont
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}


