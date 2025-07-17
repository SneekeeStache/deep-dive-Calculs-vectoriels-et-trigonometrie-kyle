// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "projectile.generated.h"

UCLASS()
class DEEPDIVEMATH_API Aprojectile : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	Aprojectile();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* TrajectorySpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileMass = 1.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LinearDamping = 0.1f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float InitialHeight = 100.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LaunchAngle = 45.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float InitialVelocity = 1000.0f;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void launchProjectile(FVector LaunchDirection=FVector::ZeroVector);
	FVector CurrentPosition;
	FVector CurrentVelocity;

	void AddTrajectoryPointsToSpline(const TArray<FVector>& points);
	TArray<FVector> CalculeTrajecory(float step, float MaxTime);
	FVector CalculeAcceleration(const FVector& velocity) const;

	
};
