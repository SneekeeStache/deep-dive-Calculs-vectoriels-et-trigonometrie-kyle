// Fill out your copyright notice in the Description page of Project Settings.

#include "projectile.h"


// Sets default values
Aprojectile::Aprojectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TrajectorySpline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetEnableGravity(false);
	ProjectileMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void Aprojectile::BeginPlay()
{
	Super::BeginPlay();
	launchProjectile();
}

// Called every frame
void Aprojectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Acceleration = CalculeAcceleration(CurrentVelocity);

	CurrentVelocity += Acceleration * DeltaTime;
	CurrentPosition += CurrentVelocity * DeltaTime + 0.5f * Acceleration * DeltaTime * DeltaTime;

	SetActorLocation(CurrentPosition);
}

void Aprojectile::launchProjectile(FVector LaunchDirection)
{
	CurrentPosition = GetActorLocation();
	CurrentPosition.Z = InitialHeight;

	FVector ForwardDir;
    
	if (!LaunchDirection.IsZero())
	{
		ForwardDir = LaunchDirection.GetSafeNormal();
	}
	else
	{
		float LaunchAngleRad = FMath::DegreesToRadians(LaunchAngle);
		ForwardDir = FVector(
			FMath::Cos(LaunchAngleRad),
			0.0f,
			FMath::Sin(LaunchAngleRad)
		);
	}
	
	CurrentVelocity = ForwardDir * InitialVelocity;
	
	SetActorLocation(CurrentPosition);
	
	const float Step = 0.05f;
	const float MaxTime = 10.0f;
	TArray<FVector> TrajPoints = CalculeTrajecory(Step, MaxTime);
	AddTrajectoryPointsToSpline(TrajPoints);
}

void Aprojectile::AddTrajectoryPointsToSpline(const TArray<FVector>& points)
{
	TrajectorySpline->ClearSplinePoints();
	for(int i = 0; i < points.Num(); ++i)
		TrajectorySpline->AddSplinePoint(points[i], ESplineCoordinateSpace::World);

	TrajectorySpline->UpdateSpline();
}

TArray<FVector> Aprojectile::CalculeTrajecory(float step, float MaxTime)
{
	TArray<FVector> Points;

	FVector SimPos = CurrentPosition;
	FVector SimVel = CurrentVelocity;

	for(float t = 0; t < MaxTime; t += step)
	{
		Points.Add(SimPos);

		FVector Acc = CalculeAcceleration(SimVel);

		SimVel += Acc * step;
		SimPos += SimVel * step + 0.5f * Acc * step * step;

		if(SimPos.Z <= 0.f)
		{
			SimPos.Z = 0.f;
			Points.Add(SimPos);
			break;
		}
	}

	return Points;
}

FVector Aprojectile::CalculeAcceleration(const FVector& velocity) const
{
	FVector Gravity(0.f, 0.f, -9.81f * ProjectileMass);
	FVector Friction = -LinearDamping * velocity;
	FVector TotalForce = Gravity + Friction;
	return TotalForce / ProjectileMass;
}

