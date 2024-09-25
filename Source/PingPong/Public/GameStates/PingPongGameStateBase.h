#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Enums/MatchState.h"
#include "PingPongGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, EMatchState, NewMatchState);

UCLASS(Abstract)
class PINGPONG_API APingPongGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_MatchState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "On Match State Changed Event")
	FOnMatchStateChangedSignature OnMatchStateChanged;

	UFUNCTION(BlueprintPure)
	EMatchState GetMatchState() const;

	UFUNCTION(BlueprintCallable)
	void SetMatchState(EMatchState InMatchState);
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_MatchState, VisibleAnywhere, BlueprintReadOnly, Category = "Ping Pong Game State | Config")
	EMatchState MatchState = EMatchState::WaitingToStart;
};
