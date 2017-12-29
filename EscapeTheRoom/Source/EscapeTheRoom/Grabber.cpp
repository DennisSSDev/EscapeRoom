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


	UE_LOG(LogTemp, Warning, TEXT("The Basic Game Mode is Initialized"));
	
	FindPhysicsComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Phys_Handle->GrabbedComponent)
	{
		FVector PlayerVLocation;
		FRotator PlayerVRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
		(
			OUT PlayerVLocation,
			OUT PlayerVRotation
		);
		FVector LineTraceEnd = PlayerVLocation + (PlayerVRotation.Vector()*Reach);

		Phys_Handle->SetTargetLocation(LineTraceEnd);
	}
	
	

}

void UGrabber::FindPhysicsComponent()
{
	Phys_Handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	Input_Comp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Phys_Handle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Physcis Component on %s could not be found"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	if (Input_Comp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"));
		//Bind controls
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
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	auto HitResult = GetPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	auto HitActor = HitResult.GetActor();
	if (HitActor)
	{
		Phys_Handle->GrabComponent
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true//allow rotation
		);
	}
	
}
void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	FVector PlayerVLocation;
	FRotator PlayerVRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerVLocation,
		OUT PlayerVRotation
	);
	FVector LineTraceEnd = PlayerVLocation + (PlayerVRotation.Vector()*Reach);

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT Hit,
		PlayerVLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor* HitActor = Hit.GetActor();
	if (HitActor != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physical object Hit detected: %s "), *HitActor->GetName());
	}
	return Hit;
}

