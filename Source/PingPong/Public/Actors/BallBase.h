#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBallBase, Log, All);

class USphereComponent;

UCLASS(Abstract)
class PINGPONG_API ABallBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball | Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnRep_TargetLocation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DrawDebugArrow(const FVector Start, const FVector End, float DotProduct, const FString& HitActorName);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Restart(const FVector InLocation, const FVector InNewVelocity);

public:	
	ABallBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	FVector GetBallVelocity() const;

	UFUNCTION(BlueprintCallable)
	void SetBallVelocity(FVector InBallVelocity);

	UFUNCTION(BlueprintCallable)
	void Restart();

	UFUNCTION(BlueprintPure)
	FVector GetRandBallVelocity() const;

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintSetter = SetBallVelocity, BlueprintGetter = GetBallVelocity, Category = "Ball | Config")
	FVector BallVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball | Config")
	FVector MinRandBallVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball | Config")
	FVector MaxRandBallVelocity;

	UPROPERTY(ReplicatedUsing = OnRep_TargetLocation, VisibleAnywhere, BlueprintReadOnly, Category = "Ball | Config")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ball | Config")
	FVector BallRestartLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ball | Config")
	float BallRestartTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball | Config")
	float MaxClientServerLocationDistance = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ball | Config")
	float DotProductTolerance = 0.01f;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
