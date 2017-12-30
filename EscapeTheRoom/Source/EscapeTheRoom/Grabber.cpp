// Free for reuse under the MIT License.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
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
	FindPhysicsComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Phys_Handle) return;
	if (Phys_Handle->GrabbedComponent != nullptr)
	{
		Phys_Handle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::FindPhysicsComponent()
{
	Phys_Handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	Input_Comp = GetOwner()->FindComponentByClass<UInputComponent>();
	if(Phys_Handle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The Physcis Component on %s could not be found"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	if (Input_Comp != nullptr)
	{
		Input_Comp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input_Comp->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Input Component on %s could not be found"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	if (!Phys_Handle) return;
	auto HitResult = GetPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	auto HitActor = HitResult.GetActor();
	if (HitActor)
	{
		Phys_Handle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab,
			NAME_None, //No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator(0.0f,0.0f,0.0f)
		);
	}
}

void UGrabber::Released()
{
	if (!Phys_Handle) return;
	Phys_Handle->ReleaseComponent();
}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physical object Hit detected: %s "), *HitActor->GetName());
	}
	return HitResult;
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerVLocation;
	FRotator PlayerVRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerVLocation,
		OUT PlayerVRotation
	);
	return PlayerVLocation + (PlayerVRotation.Vector()*Reach);
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerVLocation;
	FRotator PlayerVRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerVLocation,
		OUT PlayerVRotation
	);
	return PlayerVLocation;
}
