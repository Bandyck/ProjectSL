// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossAOEAttack.h"
#include "SLCharacter.h"

// Sets default values
ASLBossAOEAttack::ASLBossAOEAttack()
	: TickDamage(0)
	, Player(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SetActorTickInterval(1.f);
	
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollsionMesh"));
	CollisionMesh->SetCollisionProfileName(FName("OverlapAll"));
	CollisionMesh->SetHiddenInGame(true);
	CollisionMesh->SetupAttachment(RootComponent);
	
	AOEEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AOEEffect"));
	AOEEffect->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASLBossAOEAttack::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->SetActive(false);
	CollisionMesh->SetGenerateOverlapEvents(false);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionMesh->SetCollisionProfileName(FName("NoCollision"), false);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASLBossAOEAttack::OverlapBegin);
	CollisionMesh->OnComponentEndOverlap.AddDynamic(this, &ASLBossAOEAttack::OverlapEnd);
	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&,Mesh = CollisionMesh ,Actor = this]()
	{
			//CollisionMesh->SetActive(true, true);
			CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CollisionMesh->SetCollisionProfileName(FName("OverlapAll"), true);
			CollisionMesh->SetGenerateOverlapEvents(true);
	}), 0.5f, false);
}

// Called every frame
void ASLBossAOEAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Player != nullptr)
	{
		Player->TakeDamage(TickDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
	}
}

void ASLBossAOEAttack::DeactivateAOEAttack()
{
	AOEEffect->Deactivate();
	float WaitTime = 0.5f;
	GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]()
		{
			CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Player = nullptr;
		}), WaitTime, false);
	SetLifeSpan(5.f);

}


void ASLBossAOEAttack::OverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ASLCharacter>(OtherActor) != nullptr)
	{
		Player = OtherActor;
	}
}

void ASLBossAOEAttack::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ASLCharacter>(OtherActor) != nullptr)
	{
		if(Player == nullptr)
		{
			LOG_S(Error);
			return;
		}
		Player = nullptr;
	}
}

