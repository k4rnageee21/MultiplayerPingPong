#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongGoalBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalScoredSignature, AMainPlayerController*, RelatedPlayerController);

class UBoxComponent;
class AMainPlayerController;

UCLASS(Abstract)
class PINGPONG_API APongGoalBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pong Goal | Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> GoalBox;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Pong Goal | Config", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AMainPlayerController> PC;

	UFUNCTION()
	void OnGoalBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

public:	
	APongGoalBase();

	UPROPERTY(BlueprintAssignable, Category = "Goal Scored Event")
	FOnGoalScoredSignature OnGoalScored;

	UFUNCTION(BlueprintCallable)
	void SetRelatedPlayerController(AMainPlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;
};
