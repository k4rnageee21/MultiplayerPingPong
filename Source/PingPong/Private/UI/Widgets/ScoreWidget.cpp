#include "UI/Widgets/ScoreWidget.h"
#include "Components/TextBlock.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(IsValid(PlayerScore) && IsValid(OpponentScore));
}

void UScoreWidget::SetPlayerScoreText(int32 Score)
{
	check(IsValid(PlayerScore));

	PlayerScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
}

void UScoreWidget::SetOpponentScoreText(int32 Score)
{
	check(IsValid(OpponentScore));

	OpponentScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
}
