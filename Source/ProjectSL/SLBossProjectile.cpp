// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossProjectile.h"
#include "SLCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ASLBossProjectile::ASLBossProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCapsuleComponent"));
	RootComponent = Body;
	
	/*static ConstructorHelpers::FObjectFinder<UParticleSystem> PROJECTILE_PARTICLE(TEXT("/Game/FXVarietyPack/Particles/P_ky_darkStorm.P_ky_darkStorm"));
	if (PROJECTILE_PARTICLE.Succeeded())
	{
		ProjectileParticle = PROJECTILE_PARTICLE.Object;
	}*/
	

	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>("Projectile");
	
	HitParticle = CreateDefaultSubobject<UParticleSystemComponent>("Hit");
	HitParticle->SetupAttachment(RootComponent);
	HitParticle->Deactivate();
	HitParticle->SetAutoActivate(false);
	ProjectileSpeed = 1000.f;
	ProjectileDamage = 0.f;

	SetActorEnableCollision(false);
	//SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));
}

// Called when the game starts or when spawned
void ASLBossProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Body->OnComponentBeginOverlap.AddDynamic(this, &ASLBossProjectile::TornadoBeginOverlap);
	SetLifeSpan(10.0f);
}

// Called every frame
void ASLBossProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + ProjectileDir * ProjectileSpeed * DeltaTime);

}


void ASLBossProjectile::TornadoBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr)
	{
		LOG_S(Error);
		return;
	}
	/*LOG(Warning, TEXT("%s"), *OtherActor->GetName());
	if(OtherActor->IsRootComponentMovable() == false)
	{
		HitParticle->SetWorldLocation(OtherActor->GetActorLocation());
		HitParticle->SetActive(true);
		ProjectileParticle->Deactivate();
		ProjectileSpeed *= 0.2f;
	}*/
	ASLCharacter* Character = Cast<ASLCharacter>(OtherActor);

	if (Character != nullptr)
	{
		HitParticle->SetWorldLocation(OtherActor->GetActorLocation());
		HitParticle->SetActive(true);
		ProjectileParticle->Deactivate();
		ProjectileSpeed *= 0.2f;
		//HitParticle->SetWorldLocation(SweepResult.Location);
		/*HitParticle->*/
		FDamageEvent DamageEvent;
		if(GetOwner() == nullptr || GetOwner()->GetInstigatorController() == nullptr)
		{
			LOG_S(Error);
			return;
		}
		Character->TakeDamage(ProjectileDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
		SetActorEnableCollision(false);
		Cast<UCapsuleComponent>(RootComponent)->SetGenerateOverlapEvents(false);
		SetLifeSpan(5.0f);
	}
}
