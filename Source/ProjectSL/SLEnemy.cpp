// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemy.h"

#include "ProjectSL.h"
#include "SLEnemyAIController.h"
#include "SLEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "SLEnemyWidget.h"



// Sets default values
ASLEnemy::ASLEnemy() : curHP(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider"));
	//if (SK_CHARACTER.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	//	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//}
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIMINSTANCE(TEXT("/Game/Blueprints/GreaterSpiderAnimBlueprint.GreaterSpiderAnimBlueprint_C"));
	//if (ENEMY_ANIMINSTANCE.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ENEMY_ANIMINSTANCE.Class);
	//}
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SLCharacter"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/Enemy/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(120.0f, 40.0f));
		HPBarWidget->SetReceivesDecals(false);
		HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HPBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	AIControllerClass = ASLEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

}

// Called when the game starts or when spawned
void ASLEnemy::BeginPlay()
{
	Super::BeginPlay();
	curHP = EnemyData.HP;
	auto CharacterWidget = Cast<USLEnemyWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleRadius()));
		CharacterWidget->BindEnemy(this);
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->OnBeginCursorOver.AddDynamic(this, &ASLEnemy::OnBeginCursorOver);
	GetCapsuleComponent()->OnEndCursorOver.AddDynamic(this, &ASLEnemy::OnEndCursorOver);
}

void ASLEnemy::OnBeginCursorOver(UPrimitiveComponent* PrimitiveComponent)
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ASLEnemy::OnEndCursorOver(UPrimitiveComponent* PrimitiveComponent)
{
	GetMesh()->SetRenderCustomDepth(false);
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

void ASLEnemy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString DataTablePath = TEXT("/Game/Data/NormalMonsterData.NormalMonsterData");
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
	if(DataTable == nullptr)
	{
		LOG_S(Error);
		return;
	}

	FString ContextString;
	
	if(DataTable->GetRowNames().Num() < ID)
	{
		LOG_S(Error);
		return;
	}
	FSLEnemyData *EnemyDataInDT =  DataTable->FindRow<FSLEnemyData>(DataTable->GetRowNames()[ID - 1], ContextString);
	if(EnemyDataInDT == nullptr)
	{
		LOG_S(Error);
		return;
	}

	this->EnemyData = *EnemyDataInDT;
	GetCharacterMovement()->MaxWalkSpeed = this->EnemyData.WalkSpeed;
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

void ASLEnemy::Dead()
{
	USLEnemyAnimInstance* AnimInstance = Cast<USLEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	GetController()->StopMovement();
	AnimInstance->PlayDeadMontage();
	SetLifeSpan(2.0f);
	
}


float ASLEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(curHP <= 0)
	{
		return FinalDamage;
	}
	USLEnemyAnimInstance* AnimInstance = Cast<USLEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	OnAttackEnd.Broadcast();
	AnimInstance->PlayHitReaction();

	curHP -= FinalDamage;
	OnHPChange.Broadcast();

	if(curHP <= 0)
	{
		Dead();
	}
	
	return FinalDamage;
}
