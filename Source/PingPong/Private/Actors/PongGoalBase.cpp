#include "Actors/PongGoalBase.h"
#include "Components/BoxComponent.h"
#include "Actors/BallBase.h"

APongGoalBase::APongGoalBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GoalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalBox"));
	GoalBox->SetupAttachment(RootComponent);
}

void APongGoalBase::SetRelatedPlayerController(AMainPlayerController* PlayerController)
{
	if (HasAuthority())
	{
		PC = PlayerController;
	}
}

void APongGoalBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GoalBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnGoalBoxBeginOverlap);
	}
}

void APongGoalBase::OnGoalBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (IsValid(Ball))
	{
		OnGoalScored.Broadcast(PC);
	}
}