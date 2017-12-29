// Free for reuse under the MIT License.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
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
	Phys_Handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	Input_Comp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Phys_Handle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Physcis Component on %s could not be found"), *GetOwner()->GetName());
	}

	if (Input_Comp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"));
		//Bind controls
		Input_Comp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Input Component on %s could not be found"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerVLocation;
	FRotator PlayerVRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerVLocation, 
		OUT PlayerVRotation
	);

	/*UE_LOG
	(
		LogTemp, Warning, TEXT("Location: %s , Rotation: %s"),
		*PlayerVLocation.ToString(), 
		*PlayerVRotation.ToString()
	);*/
	FVector LineTraceEnd = PlayerVLocation + (PlayerVRotation.Vector()*Reach);

	DrawDebugLine
	(
		GetWorld(),
		PlayerVLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.0f
	);
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
	

}

void UGrabber::Grab()
{

}

