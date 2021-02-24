// Fill out your copyright notice in the Description page of Project Settings.


#include "SLCharacter.h"
#include "SLAnimInstance.h"
#include "SLCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "SLCharacterWidget.h"

// Sets default values
ASLCharacter::ASLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));
	if (SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/Blueprints/CharacterAnimBlueprint.CharacterAnimBlueprint_C"));
	if (CHARACTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<USLCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	Skill_Quickslot = CreateDefaultSubobject<UWidgetComponent>(TEXT("SKILLQUICKSLOT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	// Need to Integrate UI
	HPBarWidget->SetupAttachment(GetMesh());
	Skill_Quickslot->SetupAttachment(GetMesh());

	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SLCharacter"));
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	IsAttacking = false;
	IsSkilling = false;

	MaxCombo = 3;
	AttackEndComboState();

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/Character/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
		HPBarWidget->SetReceivesDecals(false);
	}
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_SKILL(TEXT("/Game/UI/Character/Skill_Icon/UI_Skill_QuickSlot.UI_Skill_QuickSlot_C"));
	if (UI_SKILL.Succeeded())
	{
		Skill_Quickslot->SetWidgetClass(UI_SKILL.Class);
		Skill_Quickslot->SetDrawSize(FVector2D(256.0f, 128.0f));
		Skill_Quickslot->SetReceivesDecals(false);
	}
	Skill_Quickslot->SetRelativeLocation(FVector(0.0f, 0.0f, -720.0f));
	Skill_Quickslot->SetWidgetSpace(EWidgetSpace::Screen);

	GetMesh()->SetReceivesDecals(false);
}

void ASLCharacter::Attack()
{
	if (IsSkilling) return;

	if (IsAttacking)
	{
		CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		CHECK(CurrentCombo == 0);
		AttackStartComboState();
		SLAnim->PlayAttackMontage();
		SLAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ASLCharacter::Skill_S()
{
	if (IsAttacking) return;

	SLAnim->PlaySkill_S_Montage();
	IsSkilling = true;
}

void ASLCharacter::Skill_F()
{
	if (IsAttacking) return;

	SLAnim->PlaySkill_F_Montage();
	IsSkilling = true;
}

void ASLCharacter::Skill_Q()
{
	if (IsAttacking) return;

	SLAnim->PlaySkill_Q_Montage();
	IsSkilling = true;
}

void ASLCharacter::Skill_W()
{
	if (IsAttacking) return;

	SLAnim->PlaySkill_W_Montage();
	IsSkilling = true;
}

void ASLCharacter::Skill_R()
{
	if (IsAttacking) return;

	SLAnim->PlaySkill_R_Montage();
	IsSkilling = true;
}

void ASLCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsAttacking);
	CHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void ASLCharacter::OnSkill_Q_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsSkilling);
	IsSkilling = false;
}

void ASLCharacter::OnSkill_W_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsSkilling);
	IsSkilling = false;
}

void ASLCharacter::OnSkill_R_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsSkilling);
	IsSkilling = false;
}

void ASLCharacter::OnSkill_S_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsSkilling);
	IsSkilling = false;
}

void ASLCharacter::OnSkill_F_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsSkilling);
	IsSkilling = false;
}

void ASLCharacter::AttackStartComboState()
{
	LOG(Warning, TEXT("%d"), CurrentCombo);
	CanNextCombo = true;
	IsComboInputOn = false;
	CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ASLCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ASLCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRange),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif // ENABLE_DRAW_DEBUG

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamegeEvent;
			LOG_S(Warning);
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamegeEvent, GetController(), this);
		}
	}
}

// Called when the game starts or when spawned
void ASLCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	auto CharacterWidget = Cast<USLCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

// Called every frame
void ASLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SLAnim = Cast<USLAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != SLAnim);

	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnAttackMontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_Q_MontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_W_MontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_R_MontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_S_MontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_F_MontageEnded);

	SLAnim->OnNextAttackCheck.AddLambda([this]()->void {
		LOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			SLAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	SLAnim->OnAttackHitCheck.AddUObject(this, &ASLCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		LOG(Warning, TEXT("OnHPIsZero"));
		SLAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});
}

float ASLCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	LOG_S(Warning);

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

// Called to bind functionality to input
void ASLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ASLCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_Q);
	PlayerInputComponent->BindAction(TEXT("Skill_W"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_W);
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_R);
	PlayerInputComponent->BindAction(TEXT("Skill_S"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_S);
	PlayerInputComponent->BindAction(TEXT("Skill_F"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_F);
}