// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemy.h"

#include "ProjectSL.h"
#include "SLEnemyAIController.h"
#include "SLEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASLEnemy::ASLEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider"));
	if (SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIMINSTANCE(TEXT("/Game/Blueprints/GreaterSpiderAnimBlueprint.GreaterSpiderAnimBlueprint_C"));
	if (ENEMY_ANIMINSTANCE.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ENEMY_ANIMINSTANCE.Class);
	}

	AIControllerClass = ASLEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	AttackRange = 200.0f;
	AttackRadius = 100.0f;
}

// Called when the game starts or when spawned
void ASLEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASLEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASLEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASLEnemy::Attack()
{
	USLEnemyAnimInstance* AnimInstance = Cast<USLEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if(AnimInstance == nullptr)
	{
		LOG_S(Error);
		return;
	}
	AnimInstance->PlayAttackMontage();
}

void ASLEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
}

float ASLEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return FinalDamage;
}
