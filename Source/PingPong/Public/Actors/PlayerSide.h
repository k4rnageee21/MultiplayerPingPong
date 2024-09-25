#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSide.generated.h"

class APongGoalBase;
class APlayerStart;

UCLASS(Abstract)
class PINGPONG_API APlayerSide : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetPlayerStartSide, meta = (AllowPrivateAccess = "true"), Category = "Player Side | Config")
	TObjectPtr<APlayerStart> PlayerStartSide = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetPongGoalSide, meta = (AllowPrivateAccess = "true"), Category = "Player Side | Config")
	TObjectPtr<APongGoalBase> PongGoalSide = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = IsOccupied, BlueprintSetter = SetOccupied, meta = (AllowPrivateAccess = "true"), Category = "Player Side | Config")
	bool bOccupied = false;
	
public:	
	APlayerSide();

	UFUNCTION(BlueprintPure)
	APongGoalBase* GetPongGoalSide() const;

	UFUNCTION(BlueprintPure)
	APlayerStart* GetPlayerStartSide() const;

	UFUNCTION(BlueprintPure)
	bool IsOccupied() const;

	UFUNCTION(BlueprintCallable)
	void SetOccupied(bool bInOccupied);

protected:
	virtual void BeginPlay() override;
};
