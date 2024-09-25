#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PingPongPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalsScoredChangedSignature, int32, NewGoalsScored);

UCLASS(Abstract)
class PINGPONG_API APingPongPlayerState : public APlayerState
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_GoalsScored();
	
public:
	UPROPERTY(BlueprintAssignable, Category = "On Goals Scored Changed Event")
	FOnGoalsScoredChangedSignature OnGoalsScoredChanged;

	UFUNCTION(BlueprintCallable)
	void SetGoalsScored(int32 InGoalsScored);

	UFUNCTION(BlueprintPure)
	int32 GetGoalsScored() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_GoalsScored, VisibleAnywhere, BlueprintGetter = GetGoalsScored, BlueprintSetter = SetGoalsScored, Category = "Ping Pong Player State | Config")
	int32 GoalsScored = 0;

	virtual void BeginPlay() override;
};
