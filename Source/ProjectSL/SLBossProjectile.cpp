// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ASLBossProjectile::ASLBossProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PROJECTILE_PARTICLE(TEXT("/Game/FXVarietyPack/Particles/P_ky_darkStorm.P_ky_darkStorm"));
	if (PROJECTILE_PARTICLE.Succeeded())
	{
		ProjectileParticle = PROJECTILE_PARTICLE.Object;
	}
	
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ATestPawnCapsuleComponent"));
	RootComponent = Body;
	//SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));
}

// Called when the game starts or when spawned
void ASLBossProjectile::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SpawnEmitterAttached(ProjectileParticle, RootComponent);
}

// Called every frame
void ASLBossProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + ProjectileDir * 1000.f * DeltaTime);

}

