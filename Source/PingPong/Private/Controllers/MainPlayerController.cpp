#include "Controllers/MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogMainPlayerController);

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	}
}

void AMainPlayerController::Move(const FInputActionValue& Value)
{
	APawn* PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn))
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector RightDirection = PossessedPawn->GetActorRightVector();
	FVector PossessedPawnInputOffset = RightDirection * RightVectorInputMultiplier * MovementVector.X;
	
	if (GetNetMode() == ENetMode::NM_Client)
	{
		PossessedPawn->SetActorLocation(PossessedPawn->GetActorLocation() + PossessedPawnInputOffset, true);
		ServerMove(PossessedPawnInputOffset);
	}
	else if (GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone)
	{
		PossessedPawn->SetActorLocation(PossessedPawn->GetActorLocation() + PossessedPawnInputOffset, true);
	}
}

void AMainPlayerController::ServerMove_Implementation(FVector PossessedPawnInputOffset)
{
	APawn* PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn))
	{
		return;
	}

	PossessedPawn->SetActorLocation(PossessedPawn->GetActorLocation() + PossessedPawnInputOffset, true);
}
