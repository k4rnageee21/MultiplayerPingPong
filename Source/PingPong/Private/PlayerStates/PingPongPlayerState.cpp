#include "PlayerStates/PingPongPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUDS/PingPongHUD.h"
#include "Kismet/GameplayStatics.h"

void APingPongPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongPlayerState, GoalsScored);
}

void APingPongPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_DedicatedServer)
	{
		UWorld* World = GetWorld();
		check(IsValid(World));

		// We do this to obtain local player controller
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		if (!IsValid(PC))
		{
			return;
		}

		APingPongHUD* HUD = PC->GetHUD<APingPongHUD>();
		if (!IsValid(HUD))
		{
			return;
		}

		HUD->LinkLocalPlayerStateToScoreWidget(this);
	}
}

void APingPongPlayerState::OnRep_GoalsScored()
{
	OnGoalsScoredChanged.Broadcast(GoalsScored);
}

void APingPongPlayerState::SetGoalsScored(int32 InGoalsScored)
{
	if (HasAuthority())
	{
		GoalsScored = InGoalsScored;
		OnRep_GoalsScored();
	}
}

int32 APingPongPlayerState::GetGoalsScored() const
{
	return GoalsScored;
}
