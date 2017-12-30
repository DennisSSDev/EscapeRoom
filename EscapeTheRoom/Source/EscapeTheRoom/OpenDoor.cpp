// Free for reuse under the MIT License.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!Owner) UE_LOG(LogTemp, Error, TEXT("The Owner Has not been initialized in OpenDoor.cpp"));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PressurePlate || !Owner) return;

	if (TotalMassOnPlate() > TriggerMass)
		OnOpen.Broadcast();
	else
		OnClose.Broadcast();
	
}

float UOpenDoor::TotalMassOnPlate() const
{
	float TotalMass = 0.f;
	TSet<AActor*> ActorPtrs;
	PressurePlate->GetOverlappingActors(OUT ActorPtrs);
	for (const auto* Actor : ActorPtrs)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is overlapping"), *Actor->GetName());
	}
	return TotalMass;
}
