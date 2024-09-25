#include "Actors/PlayerSide.h"
#include "Actors/PongGoalBase.h"

APlayerSide::APlayerSide()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void APlayerSide::BeginPlay()
{
	Super::BeginPlay();
	

}

APongGoalBase* APlayerSide::GetPongGoalSide() const
{
	return PongGoalSide;
}

APlayerStart* APlayerSide::GetPlayerStartSide() const
{
	return PlayerStartSide;
}

bool APlayerSide::IsOccupied() const
{
	return bOccupied;
}

void APlayerSide::SetOccupied(bool bInOccupied)
{
	bOccupied = bInOccupied;
}
