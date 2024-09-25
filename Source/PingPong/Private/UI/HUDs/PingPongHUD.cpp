#include "UI/HUDs/PingPongHUD.h"
#include "UI/Widgets/WaitingMatchStartWidget.h"
#include "GameStates/PingPongGameStateBase.h"
#include "UI/Widgets/ScoreWidget.h"
#include "PlayerStates/PingPongPlayerState.h"

void APingPongHUD::BeginPlay()
{
	Super::BeginPlay();

    UWorld* World = GetWorld();
    check(IsValid(World));

    APingPongGameStateBase* PingPongGS = World->GetGameState<APingPongGameStateBase>();
    if (!IsValid(PingPongGS))
    {
        return;
    }

    ShowScoreWidget();

    if (PingPongGS->GetMatchState() == EMatchState::WaitingToStart)
    {
        ShowWaitingMatchStartWidget();

        if (IsValid(WaitingMatchStartWidget))
        {
            PingPongGS->OnMatchStateChanged.AddDynamic(this, &ThisClass::HandleMatchStateChanged);
        }
    }
}

void APingPongHUD::ShowWaitingMatchStartWidget()
{
    if (WaitingMatchStartWidget != nullptr || !IsValid(WaitingMatchStartWidgetClass))
    {
        return;
    }

    UWorld* World = GetWorld();
    check(IsValid(World));

    WaitingMatchStartWidget = CreateWidget<UWaitingMatchStartWidget>(World, WaitingMatchStartWidgetClass);
    if (IsValid(WaitingMatchStartWidget))
    {
        WaitingMatchStartWidget->AddToViewport(WaitingMatchStartWidgetOrderZ);
    }
}

void APingPongHUD::HandleMatchStateChanged(EMatchState NewMatchState)
{
    if (NewMatchState == EMatchState::InProgress)
    {
        CloseWaitingMatchStartWidget();
    }
}

void APingPongHUD::LinkLocalPlayerStateToScoreWidget(APingPongPlayerState* PingPongPlayerState)
{
    // We assume that there are only 2 players in the game
    APlayerController* PlayerController = PingPongPlayerState->GetPlayerController();
    APlayerController* OwningPlayerController = GetOwningPlayerController();

    if (IsValid(PlayerController) && IsValid(OwningPlayerController) && PlayerController == OwningPlayerController)
    {
        // It means that Player State is owned by local PC, so it's Player
        PingPongPlayerState->OnGoalsScoredChanged.AddDynamic(this, &ThisClass::UpdatePlayerScoreWidget);
    }
    else
    {
        // It means that Player State isn't owned by local PC, so it's Opponent
        PingPongPlayerState->OnGoalsScoredChanged.AddDynamic(this, &ThisClass::UpdateOpponentScoreWidget);
    }
}

void APingPongHUD::UpdatePlayerScoreWidget(int32 NewPlayerScore)
{
    if (IsValid(ScoreWidget))
    {
        ScoreWidget->SetPlayerScoreText(NewPlayerScore);
    }
}

void APingPongHUD::UpdateOpponentScoreWidget(int32 NewOpponentScore)
{
    if (IsValid(ScoreWidget))
    {
        ScoreWidget->SetOpponentScoreText(NewOpponentScore);
    }
}

void APingPongHUD::CloseWaitingMatchStartWidget()
{
    if (IsValid(WaitingMatchStartWidget))
    {
        WaitingMatchStartWidget->RemoveFromParent();
        WaitingMatchStartWidget = nullptr;
    }
}

void APingPongHUD::ShowScoreWidget()
{
    if (ScoreWidget != nullptr || !IsValid(ScoreWidgetClass))
    {
        return;
    }

    UWorld* World = GetWorld();
    check(IsValid(World));

    ScoreWidget = CreateWidget<UScoreWidget>(World, ScoreWidgetClass);
    if (IsValid(ScoreWidget))
    {
        ScoreWidget->AddToViewport(ScoreWidgetOrderZ);
    }
}

void APingPongHUD::CloseScoreWidget()
{
    if (IsValid(ScoreWidget))
    {
        ScoreWidget->RemoveFromParent();
        ScoreWidget = nullptr;
    }
}
