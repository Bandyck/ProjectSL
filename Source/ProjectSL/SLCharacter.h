// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "SLFadeComponent.h"
#include "GameFramework/Character.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "SLCharacter.generated.h"

UCLASS()
class PROJECTSL_API ASLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLCharacter();

	void Attack();
	void Skill_Q();
	void Skill_W();
	void Skill_R();
	void Skill_S();
	void Skill_F();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_Q_MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_W_MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_R_MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_S_MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_F_MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void AttackStartComboState();
	void AttackEndComboState();
	
	void AttackCheck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class USLCharacterStatComponent* CharacterStat;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USLFadeComponent* FadeComponent;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* Skill_Quickslot;

	bool IsMovable()
	{
		if(IsAttacking)
		{
			return false;
		}
		if(IsSkilling)
		{
			return false;
		}
		return true;
	}
private:
	UNiagaraSystem* WeakAttackEffect;
	UNiagaraSystem* PowerAttackEffect;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY()
		class USLAnimInstance* SLAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY()
		bool IsSkilling;
};
