#include "GameStates/PingPongGameStateBase.h"
#include "Net/UnrealNetwork.h"

void APingPongGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongGameStateBase, MatchState);
}

EMatchState APingPongGameStateBase::GetMatchState() const
{
	return MatchState;
}

void APingPongGameStateBase::SetMatchState(EMatchState InMatchState)
{
	if (HasAuthority())
	{
		MatchState = InMatchState;
		OnRep_MatchState();
	}
}

void APingPongGameStateBase::OnRep_MatchState()
{
	OnMatchStateChanged.Broadcast(MatchState);
}