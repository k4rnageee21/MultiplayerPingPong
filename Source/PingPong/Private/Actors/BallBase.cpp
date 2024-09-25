#include "Actors/BallBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogBallBase);

ABallBase::ABallBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	bReplicates = true;
}

void ABallBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABallBase, BallVelocity);
	DOREPLIFETIME(ABallBase, TargetLocation);
}

void ABallBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Mesh->OnComponentHit.AddDynamic(this, &ThisClass::OnMeshHit);
		BallRestartLocation = GetActorLocation();
	}
	else
	{
		TargetLocation = GetActorLocation();
	}
}

void ABallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector TickBallOffset = BallVelocity * DeltaTime;
		AddActorWorldOffset(TickBallOffset, true);
		TargetLocation = GetActorLocation();
	}
	else
	{
		// Client Location Simulation
		FVector NewClientLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, BallVelocity.Length());
		SetActorLocation(NewClientLocation, true);
	}
}

FVector ABallBase::GetBallVelocity() const
{
	return BallVelocity;
}

void ABallBase::SetBallVelocity(FVector InBallVelocity)
{
	BallVelocity = InBallVelocity;
}

void ABallBase::Restart()
{
	if (HasAuthority())
	{
		FVector NewRandVelocity = GetRandBallVelocity();
		Multicast_Restart(BallRestartLocation, NewRandVelocity);
	}
}

FVector ABallBase::GetRandBallVelocity() const
{
	float RandomX = FMath::RandRange(MinRandBallVelocity.X, MaxRandBallVelocity.X);
	float RandomY = FMath::RandRange(MinRandBallVelocity.Y, MaxRandBallVelocity.Y);
	RandomX *= FMath::RandBool() ? 1.f : -1.f;
	RandomY *= FMath::RandBool() ? 1.f : -1.f;

	return FVector(RandomX, RandomY, 0.f);
}

void ABallBase::Multicast_Restart_Implementation(const FVector InLocation, const FVector InNewVelocity)
{
	SetActorLocation(InLocation);
	TargetLocation = GetActorLocation();
	BallVelocity = InNewVelocity;
}

void ABallBase::Multicast_DrawDebugArrow_Implementation(const FVector Start, const FVector End, float DotProduct, const FString& HitActorName)
{
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), Start, End, 5.f, FLinearColor::Black, 5.f, 10.f);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(
		TEXT("DotProduct: %f, Hit Actor Name: %s"), DotProduct, *HitActorName
	));
}

void ABallBase::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority() || Hit.GetActor() == this)
	{
		return;
	}

	FVector2D HitSurfaceNormal2D;
	HitSurfaceNormal2D.X = Hit.ImpactNormal.X;
	HitSurfaceNormal2D.Y = Hit.ImpactNormal.Y;

	FVector2D BallForwardVector2D;
	BallForwardVector2D.X = GetActorForwardVector().X;
	BallForwardVector2D.Y = GetActorForwardVector().Y;

	float DotProduct2D = UKismetMathLibrary::DotProduct2D(HitSurfaceNormal2D, BallForwardVector2D);
	//Multicast_DrawDebugArrow(Hit.Location, Hit.Location + Hit.ImpactNormal * 200.f, DotProduct2D, Hit.GetActor()->GetName());
	if (FMath::IsNearlyEqual(DotProduct2D, 0.f, DotProductTolerance))
	{
		// This condition means that ball hit vertical surface
		BallVelocity.Y = -BallVelocity.Y;
	}
	else if (FMath::IsNearlyEqual(FMath::Abs(DotProduct2D), 1.f, DotProductTolerance))
	{
		// This condition means that ball hit horizontal surface
		BallVelocity.X = -BallVelocity.X;
	}
	else
	{
		// There are no non-horizontal or non-vertical surfaces in this pong implementation
		UE_LOG(LogBallBase, Warning, TEXT("Wrong hit surface shape, consider changing the shape of the surface to horizontal or vertical"));
	}
}

void ABallBase::OnRep_TargetLocation()
{
	// If the Client Location far away from the Server Location
	// due to certain conditions: high ping, lag on the user's device, etc,
	// set the Client Location to Target Location
	if (!HasAuthority())
	{
		if (MaxClientServerLocationDistance >= (GetActorLocation() - TargetLocation).Size())
		{
			SetActorLocation(TargetLocation);
		}
	}
}