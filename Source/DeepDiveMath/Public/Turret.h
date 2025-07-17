// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "projectile.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class AturretGamemode;

UCLASS()
class DEEPDIVEMATH_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();
	UPROPERTY(EditAnywhere,Category="Visuel")
	UStaticMeshComponent* TurretBody;
	UPROPERTY(EditAnywhere,Category="Visuel")
	UStaticMeshComponent* TurretHead;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="Parameters")
	float SweepAmplitudeDeg = 60.0f;
	UPROPERTY(EditAnywhere,Category="Parameters")
	float SweepSpeed = 1.f;
	UPROPERTY(EditAnywhere,Category="Parameters")
	float MaxAngularSpeedDeg = 90.f;
	UPROPERTY(EditAnywhere,Category="Parameters")
	float FieldOfViewDeg = 45.f;
	UPROPERTY(EditAnywhere,Category="Parameters")
	float ProjectileSpeed = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float headSpawnForwardOffset=100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float timerShooting=2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<Aprojectile> projectileToSpawn;
	
	float BaseYawDeg = 0.0f;
	float CurrentYawDeg = 0.f;
	AActor* CurrentTarget = nullptr;

	FTimerHandle TimerHandle;

	void SweepSearch(float DeltaTime);
	AActor* FindTarget();
	void TrackTarget(float DeltaTime);
	void FireProjectile();

	bool IsTargetInSight(AActor* Target) const;
	bool IsAlignedWithTarget(AActor* Target, float ToleranceDeg = 3.f) const;
	void RotateTowards(const FVector& TargetDir, float DeltaTime);

	AturretGamemode* Gamemode;

	FVector GetForwardVectorWithMath(const FRotator& ActorRotator) const;

};
