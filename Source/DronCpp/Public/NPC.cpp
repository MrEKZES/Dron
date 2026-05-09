#include "NPC.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ANPC::MoveToRandomLocation, 5.0f, true, 0.5f);
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC::MoveToRandomLocation()
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (!AIC) return;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) return;

	FNavLocation RandomLocation;
	bool bFound = NavSys->GetRandomReachablePointInRadius(GetActorLocation(), 1500.0f, RandomLocation);
    
	if (bFound)
	{
		AIC->MoveToLocation(RandomLocation.Location);
	}
}