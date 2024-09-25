#include "GameModes/PingPongGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/PlayerSide.h"
#include "Actors/PongGoalBase.h"
#include "Controllers/MainPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Actors/BallBase.h"
#include "GameStates/PingPongGameStateBase.h"
#include "PlayerStates/PingPongPlayerState.h"

DEFINE_LOG_CATEGORY(LogPingPongGameModeBase);

void APingPongGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    FindPlayerSides();
    FindBall();

    APingPongGameStateBase* PingPongGS = GetGameState<APingPongGameStateBase>();
    if (!IsValid(PingPongGS))
    {
        UE_LOG(LogPingPongGameModeBase, Warning, TEXT("Game State class doesn't match the Game Mode"));
        return;
    }
    
    PingPongGS->OnMatchStateChanged.AddDynamic(this, &ThisClass::HandleMatchStateChanged);

    UWorld* World = GetWorld();
    check(IsValid(World));

    FTimerHandle TH;
    World->GetTimerManager().SetTimer(TH, this, &ThisClass::CheckIfShouldStartMatch, CheckIfShouldStartMatchCountdown, false);
}

void APingPongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UWorld* World = GetWorld();
    check(IsValid(World));

    FTimerHandle TH;
    World->GetTimerManager().SetTimer(TH, this, &ThisClass::CheckIfShouldStartMatch, CheckIfShouldStartMatchCountdown, false);

    if (FirstPlayerController == nullptr)
    {
        FirstPlayerController = Cast<AMainPlayerController>(NewPlayer);
    }
    else if (SecondPlayerController == nullptr)
    {
        SecondPlayerController = Cast<AMainPlayerController>(NewPlayer);
    }
}

void APingPongGameModeBase::FindPlayerSides()
{
    UWorld* World = GetWorld();
    check(IsValid(World));
    TArray<AActor*> FoundActors;

    UGameplayStatics::GetAllActorsOfClass(World, PlayerSideClass, FoundActors);
    for (AActor* Actor : FoundActors)
    {
        APlayerSide* PlayerSideActor = Cast<APlayerSide>(Actor);
        if (IsValid(PlayerSideActor) && !PlayerSideActor->IsOccupied())
        {
            PlayerSideActors.Add(PlayerSideActor);
        }
    }
}

void APingPongGameModeBase::FindBall()
{
    UWorld* World = GetWorld();
    check(IsValid(World));
    TArray<AActor*> FoundActors;

    UGameplayStatics::GetAllActorsOfClass(World, BallClass, FoundActors);
    for (AActor* Actor : FoundActors)
    {
        ABallBase* FoundBall = Cast<ABallBase>(Actor);
        if (IsValid(FoundBall))
        {
            Ball = FoundBall;
            return;
        }
    }

    Ball = nullptr;
    UE_LOG(LogPingPongGameModeBase, Warning, TEXT("Can't find the ball"));
}

void APingPongGameModeBase::CheckIfShouldStartMatch()
{
    APingPongGameStateBase* PingPongGS = GetGameState<APingPongGameStateBase>();
    if (!IsValid(PingPongGS))
    {
        UE_LOG(LogPingPongGameModeBase, Warning, TEXT("Game State class doesn't match the Game Mode"));
        return;
    }

    if (GetNumPlayers() >= PlayersToStartMatch && PingPongGS->GetMatchState() != EMatchState::InProgress)
    {
        PingPongGS->SetMatchState(EMatchState::InProgress);
    }
}

void APingPongGameModeBase::HandleGoalScored(AMainPlayerController* PlayerControllerThatMissed)
{
    if (!IsValid(Ball))
    {
        return;
    }

    Ball->Restart();

    if (!IsValid(PlayerControllerThatMissed) || !IsValid(FirstPlayerController) || !IsValid(SecondPlayerController))
    {
        return;
    }

    AMainPlayerController* PlayerControllerThatScored = 
        PlayerControllerThatMissed == FirstPlayerController ? SecondPlayerController : FirstPlayerController;
    
    APingPongPlayerState* PlayerState = PlayerControllerThatScored->GetPlayerState<APingPongPlayerState>();
    if (IsValid(PlayerState))
    {
        PlayerState->SetGoalsScored(PlayerState->GetGoalsScored() + 1);
    }
}

void APingPongGameModeBase::HandleMatchStateChanged(EMatchState NewMatchState)
{
    if (NewMatchState == EMatchState::InProgress)
    {
        if (!IsValid(Ball))
        {
            return;
        }

        Ball->Restart();
    }
}

AActor* APingPongGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    if (PlayerSideActors.Num() <= 0)
    {
        FindPlayerSides();
    }

    if (PlayerSideActors.Num() <= 0)
    {
        return nullptr;
    }

    APlayerSide* PlayerSideActor = PlayerSideActors.Pop();
    if (IsValid(PlayerSideActor))
    {
        PlayerSideActor->SetOccupied(true);
        AMainPlayerController* PC = Cast<AMainPlayerController>(Player);
        if (IsValid(PC))
        {
            APongGoalBase* PongGoal = PlayerSideActor->GetPongGoalSide();
            if (IsValid(PongGoal))
            {
                PongGoal->SetRelatedPlayerController(PC);
                PongGoal->OnGoalScored.AddDynamic(this, &ThisClass::HandleGoalScored);
            }
        }
        return PlayerSideActor->GetPlayerStartSide();
    }

    return nullptr;
}
