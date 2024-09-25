#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogMainPlayerController, Log, All);

UCLASS(Abstract)
class PINGPONG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main Player Controller | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main Player Controller | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UFUNCTION(Server, Reliable)
	void ServerMove(FVector PossessedPawnInputOffset);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Player Controller | Config")
	float RightVectorInputMultiplier = 5.f;

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;
};
