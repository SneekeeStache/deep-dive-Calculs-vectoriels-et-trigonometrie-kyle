// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "projectile.h"
#include "turretGamemode.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    TurretBody = CreateDefaultSubobject<UStaticMeshComponent>("TurretBody");
    TurretHead = CreateDefaultSubobject<UStaticMeshComponent>("Head");
    RootComponent = TurretBody;
    TurretHead->SetupAttachment(TurretBody);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
    BaseYawDeg = TurretHead->GetComponentRotation().Yaw;
    Gamemode = Cast<AturretGamemode>(GetWorld()->GetAuthGameMode());
    GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&ATurret::FireProjectile,timerShooting,true);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!CurrentTarget) {
        SweepSearch(DeltaTime);
        CurrentTarget = FindTarget();
        
    }
    else {
        TrackTarget(DeltaTime);
    }
}

void ATurret::SweepSearch(float DeltaTime) {
    float AngleRad = FMath::DegreesToRadians(SweepAmplitudeDeg) * FMath::Sin(SweepSpeed * GetWorld()->GetTimeSeconds());
    CurrentYawDeg = BaseYawDeg + FMath::RadiansToDegrees(AngleRad);
    TurretHead->SetRelativeRotation(FRotator(0, CurrentYawDeg, 0));
}

AActor* ATurret::FindTarget() {
    for (AActor* Candidate : Gamemode->Targets) {
        if (IsTargetInSight(Candidate)) {
            return Candidate;
        }
    }
    return nullptr;
}

bool ATurret::IsTargetInSight(AActor* Target) const {
    FVector DirToTarget = (Target->GetActorLocation() - TurretHead->GetComponentLocation()).GetSafeNormal();
    FVector Forward = GetForwardVectorWithMath(TurretHead->GetComponentRotation());
    float CosAngle = FVector::DotProduct(Forward, DirToTarget);
    float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(CosAngle));
    return AngleDeg <= FieldOfViewDeg * 0.5f;
}

void ATurret::TrackTarget(float DeltaTime) {
    FVector DirToTarget = (CurrentTarget->GetActorLocation() - TurretHead->GetComponentLocation()).GetSafeNormal();
    RotateTowards(DirToTarget, DeltaTime);

    if (!IsTargetInSight(CurrentTarget)) {
        CurrentTarget = nullptr;
    }
}

void ATurret::FireProjectile()
{
    if (!CurrentTarget) return;
    FVector ForwardDirection = GetForwardVectorWithMath(TurretHead->GetComponentRotation());
    Aprojectile* Projectile = GetWorld()->SpawnActor<Aprojectile>(projectileToSpawn,TurretHead->GetComponentLocation()+(ForwardDirection*headSpawnForwardOffset),TurretHead->GetComponentRotation());
    if (Projectile)
    {
        Projectile->launchProjectile(ForwardDirection);
    }
}

void ATurret::RotateTowards(const FVector& TargetDir, float DeltaTime) {
    FRotator DesiredRot = TargetDir.Rotation();
    FRotator CurrentRot = TurretHead->GetComponentRotation();
    float MaxDelta = MaxAngularSpeedDeg * DeltaTime;

    float YawDelta = FMath::FindDeltaAngleDegrees(CurrentRot.Yaw, DesiredRot.Yaw);
    float AppliedDelta = FMath::Clamp(YawDelta, -MaxDelta, MaxDelta);
    
    float NewYaw = CurrentRot.Yaw + AppliedDelta;
    float RelativeYaw = FMath::FindDeltaAngleDegrees(BaseYawDeg, NewYaw);
    
    float ClampedRelativeYaw = FMath::Clamp(RelativeYaw, -SweepAmplitudeDeg, SweepAmplitudeDeg);
    
    float ClampedYaw = BaseYawDeg + ClampedRelativeYaw;

    TurretHead->SetRelativeRotation(FRotator(0.f, ClampedYaw, 0.f));
    CurrentYawDeg = ClampedYaw;
}

FVector ATurret::GetForwardVectorWithMath(const FRotator& ActorRotator) const
{
    float Pitch = FMath::DegreesToRadians(ActorRotator.Pitch);
    float Yaw = FMath::DegreesToRadians(ActorRotator.Yaw);

    float CosPitch = FMath::Cos(Pitch);
    float SinPitch = FMath::Sin(Pitch);
    float CosYaw = FMath::Cos(Yaw);
    float SinYaw = FMath::Sin(Yaw);
    
    FVector Forward;
    Forward.X = CosPitch * CosYaw;
    Forward.Y = CosPitch * SinYaw;
    Forward.Z = SinPitch;
    return Forward;
}


bool ATurret::IsAlignedWithTarget(AActor* Target, float ToleranceDeg) const {
    FVector DirToTarget = (Target->GetActorLocation() - TurretHead->GetComponentLocation()).GetSafeNormal();
    FVector Forward = GetForwardVectorWithMath(TurretHead->GetComponentRotation());
    float CosAngle = FVector::DotProduct(Forward, DirToTarget);
    float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(CosAngle));
    return AngleDeg < ToleranceDeg;
}





