#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Enums/MatchState.h"
#include "PingPongHUD.generated.h"

class UWaitingMatchStartWidget;
class UScoreWidget;
class APingPongPlayerState;

UCLASS(Abstract)
class PINGPONG_API APingPongHUD : public AHUD
{
	GENERATED_BODY()

	UFUNCTION()
	void HandleMatchStateChanged(EMatchState NewMatchState);

	UFUNCTION()
	void UpdatePlayerScoreWidget(int32 NewPlayerScore);

	UFUNCTION()
	void UpdateOpponentScoreWidget(int32 NewOpponentScore);

public:
	UFUNCTION(BlueprintCallable)
	void ShowWaitingMatchStartWidget();

	UFUNCTION(BlueprintCallable)
	void CloseWaitingMatchStartWidget();

	UFUNCTION(BlueprintCallable)
	void ShowScoreWidget();

	UFUNCTION(BlueprintCallable)
	void CloseScoreWidget();

	UFUNCTION(BlueprintCallable)
	void LinkLocalPlayerStateToScoreWidget(APingPongPlayerState* PingPongPlayerState);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ping Pong HUD | Widgets")
	TSubclassOf<UWaitingMatchStartWidget> WaitingMatchStartWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ping Pong HUD | Widgets")
	TObjectPtr<UWaitingMatchStartWidget> WaitingMatchStartWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ping Pong HUD | Widgets")
	int32 WaitingMatchStartWidgetOrderZ = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ping Pong HUD | Widgets")
	TSubclassOf<UScoreWidget> ScoreWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ping Pong HUD | Widgets")
	TObjectPtr<UScoreWidget> ScoreWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ping Pong HUD | Widgets")
	int32 ScoreWidgetOrderZ = 1;

	virtual void BeginPlay() override;
};
