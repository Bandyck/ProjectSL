// Fill out your copyright notice in the Description page of Project Settings.


#include "SLCharacter.h"
#include "SLAnimInstance.h"
#include "SLCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "SLCharacterWidget.h"
#include "SLEnemy.h"
#include "SLSkillSlot.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASLCharacter::ASLCharacter()
	: InteractableActor(nullptr)
	, needTurn(false)
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
	FadeComponent = CreateDefaultSubobject<USLFadeComponent>(TEXT("FadeComponent"));
	/*Skill_Quickslot = CreateDefaultSubobject<UWidgetComponent>(TEXT("SKILLQUICKSLOT"));*/

	SpringArm->SetupAttachment(GetCapsuleComponent());
	//HPBarWidget->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	// Need to Integrate UI
	//HPBarWidget->SetupAttachment(GetMesh());
	//Skill_Quickslot->SetupAttachment(GetMesh());

	SpringArm->TargetArmLength = 1500.0f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, -135.0f, 0.0f));
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
		HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		//HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
		HPBarWidget->SetReceivesDecals(false);

	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> SKILL_ICON(TEXT("Texture2D'/Game/UI_Kit/Textures/icons/icon_spell_13.icon_spell_13'"));
	if(SKILL_ICON.Succeeded())
	{
		Skill_1_Icon = SKILL_ICON.Object;
		Skill_1_CoolTime = 3.f;
		Skill_1_Damage = 20.f;
		Skill_1_MPConsumption = 30.f;
	}
	//HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));

	/*static ConstructorHelpers::FClassFinder<UUserWidget> UI_SKILL(TEXT("/Game/UI/Character/Skill_Icon/UI_Skill_QuickSlot.UI_Skill_QuickSlot_C"));
	if (UI_SKILL.Succeeded())
	{
		Skill_Quickslot->SetWidgetClass(UI_SKILL.Class);
		Skill_Quickslot->SetDrawSize(FVector2D(256.0f, 128.0f));
		Skill_Quickslot->SetReceivesDecals(false);
	}
	Skill_Quickslot->SetRelativeLocation(FVector(0.0f, 0.0f, -720.0f));
	Skill_Quickslot->SetWidgetSpace(EWidgetSpace::Screen);*/
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetReceivesDecals(false);
	GetCapsuleComponent()->SetHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ASLCharacter::Attack()
{
	if (InteractableActor != nullptr) return;
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

void ASLCharacter::Dodge()
{
	if(IsSkilling == true)
	{
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		HitResult.Location.Z = GetActorLocation().Z;
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location);
		SetActorRotation(LookAtRotation);
	}
	SLAnim->StopAllMontages(0.0f);
	GetController()->StopMovement();
	SLAnim->PlayDodgeMontage();
	IsSkilling = true;
}


void ASLCharacter::Skill_S()
{
	if (Slots[0]->IsOnCooldown() || CharacterStat->GetCurrentMP() < Skill_1_MPConsumption || IsSkilling)
	{
		return;
	}
	
	SLAnim->StopAllMontages(0.0f);
	GetController()->StopMovement();
	CharacterStat->ConsumeMP(Skill_1_MPConsumption);
	Slots[0]->UseSlot();
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
	if(Montage != SLAnim->GetAttackMontage())
	{
		return;
	}
	CHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void ASLCharacter::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage != SLAnim->GetDodgeMontage())
	{
		return;
	}
	IsSkilling = false;
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
	if(Montage != SLAnim->GetSkillSMontage())
	{
		return;
	}
	IsSkilling = false;
}

void ASLCharacter::OnSkill_F_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CHECK(IsSkilling);
	IsSkilling = false;
}

void ASLCharacter::AttackStartComboState()
{
	GetController()->StopMovement();
	FHitResult HitResult;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		HitResult.Location.Z = GetActorLocation().Z;
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location);
		TargetRotator = LookAtRotation;
		needTurn = true;
	}
	//LOG(Warning, TEXT("%d"), CurrentCombo);
	CanNextCombo = true;
	IsComboInputOn = false;
	CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ASLCharacter::AttackEndComboState()
{
	needTurn = false;
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ASLCharacter::AttackCheck()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRange),
		Params
	);
	
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;


	DrawDebugSphere(GetWorld(),
		Center,
		HalfHeight,
		16,
		DrawColor,
		false,
		DebugLifeTime);
	
	/*DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);*/

#endif // ENABLE_DRAW_DEBUG
	
	if (bResult)
	{
		FDamageEvent DamageEvent;
		for (auto HitResult : HitResults)
		{
			FVector dir = HitResult.Actor->GetActorLocation() - GetActorLocation();
			dir.Normalize();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.ImpactPoint).Add(0, 90, 0));
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}

void ASLCharacter::Skill1Check()
{
	AActor* Player = this;
	if (Player == nullptr)
	{
		LOG_S(Error);
		return;
	}

	float Skill_S_Range = 500.f;

	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params(NAME_None, nullptr, Player);

	bool bResult = Player->GetWorld()->SweepMultiByChannel(
		HitResults,
		Player->GetActorLocation(),
		Player->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Skill_S_Range),
		Params
	);
#if ENABLE_DRAW_DEBUG
	FVector Center = Player->GetActorLocation();
	float HalfHeight = Skill_S_Range;
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;


	DrawDebugSphere(GetWorld(),
		Center,
		HalfHeight,
		16,
		DrawColor,
		false,
		DebugLifeTime);

	/*DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);*/

#endif // ENABLE_DRAW_DEBUG

	if (bResult)
	{
		FDamageEvent DamageEvent;
		for (auto HitResult : HitResults)
		{
			HitResult.Actor->TakeDamage(Skill_1_Damage, DamageEvent, GetController(), this);

		}
	}
}


// Called when the game starts or when spawned
void ASLCharacter::BeginPlay()
{
	Super::BeginPlay();

	HitEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Hack_And_Slash_FX/VFX_Niagara/Impacts/NS_Fire_Slash_Impact.NS_Fire_Slash_Impact"), nullptr, LOAD_None, nullptr);


	auto CharacterWidget = Cast<USLCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		//CharacterWidget->AddToViewport();
		HPBarWidget->SetVisibility(false);
		CharacterWidget->AddToViewport();
		CharacterWidget->BindCharacterStat(CharacterStat);
	USLSkillSlot* Slot1 = Cast<USLSkillSlot>(HPBarWidget->GetUserWidgetObject()->GetWidgetFromName(FName("Slot1")));
		if (Slot1 == nullptr)
		{
			LOG_S(Error);
			return;
		}
		Slot1->SetNewContent(Skill_1_Icon, Skill_1_CoolTime);
		Slots.Add(Slot1);
	}

	/*FOutputDeviceNull OutputDevice;
	if(!Slot1->CallFunctionByNameWithArguments(TEXT("SetNewContent Slot1"), OutputDevice, nullptr, true))
	{
		LOG_S(Error);
	}
	if(!Slot1->CallFunctionByNameWithArguments(TEXT("UseSlot Slot1"), OutputDevice, nullptr, true))
	{
		LOG_S(Error);
	}*/
	
}

// Called every frame
void ASLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(needTurn == true)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotator, DeltaTime, 10.0f));
		if(GetActorRotation().Equals(TargetRotator, 0.01f))
		{
			needTurn = false;
		}
	}
}

void ASLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SLAnim = Cast<USLAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != SLAnim);

	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnAttackMontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_S_MontageEnded);
	SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnDodgeMontageEnded);
	//SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_Q_MontageEnded);
	//SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_W_MontageEnded);
	//SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_R_MontageEnded);
	//SLAnim->OnMontageEnded.AddDynamic(this, &ASLCharacter::OnSkill_F_MontageEnded);
	
	SLAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			SLAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	SLAnim->OnAttackHitCheck.AddUObject(this, &ASLCharacter::AttackCheck);
	SLAnim->OnSkill1HitCheck.AddUObject(this, &ASLCharacter::Skill1Check);

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
	PlayerInputComponent->BindAction(TEXT("Skill_S"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_S);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ASLCharacter::Dodge);
	/*PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_Q);
	PlayerInputComponent->BindAction(TEXT("Skill_W"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_W);
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_R);
	PlayerInputComponent->BindAction(TEXT("Skill_F"), EInputEvent::IE_Pressed, this, &ASLCharacter::Skill_F);*/
}