#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enums/MatchState.h"
#include "PingPongGameModeBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPingPongGameModeBase, Log, All);

class APlayerSide;
class ABallBase;
class AMainPlayerController;

UCLASS(Abstract)
class PINGPONG_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	TSubclassOf<APlayerSide> PlayerSideClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	TSubclassOf<ABallBase> BallClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	TArray<TObjectPtr<APlayerSide>> PlayerSideActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	TObjectPtr<ABallBase> Ball;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	TObjectPtr<AMainPlayerController> FirstPlayerController = nullptr;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	TObjectPtr<AMainPlayerController> SecondPlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	int32 PlayersToStartMatch = 2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ping Pong Game Mode | Config")
	float CheckIfShouldStartMatchCountdown = 2.f;

	void FindPlayerSides();

	void FindBall();

	void CheckIfShouldStartMatch();

	UFUNCTION()
	void HandleGoalScored(AMainPlayerController* PlayerControllerThatMissed);

	UFUNCTION()
	void HandleMatchStateChanged(EMatchState NewMatchState);
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
