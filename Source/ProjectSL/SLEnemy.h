// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"

#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "SLEnemy.generated.h"

USTRUCT(BlueprintType)
struct FSLEnemyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FSLEnemyData() : ID(0), Name("NoName"), HP(0), AttackPower(0), AttackRange(0), AttackComboCount(0),RecognitionRange(0), WalkSpeed(0)
	{}
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	uint8 ID;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	float HP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	float AttackPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	float AttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	uint8 AttackComboCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	float RecognitionRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	float WalkSpeed;
};


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS(Blueprintable)
class PROJECTSL_API ASLEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void Attack();
	void Dead();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* PrimitiveComponent);
	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* PrimitiveComponent);
public:	
	FOnAttackEndDelegate OnAttackEnd;
	FOnHPChangedDelegate OnHPChange;

	float GetAttackRange() const { return EnemyData.AttackRange; }
	void SetAttackRange(float attackRange) { EnemyData.AttackRange = attackRange; }

	float GetAttackRadius() const { return EnemyData.AttackRange; }
	void SetAttackRadius(float attackRadius) { EnemyData.AttackRange = attackRadius; }

	float GetAttackPower() const { return EnemyData.AttackPower;  }

	float GetAttackComboCount() const { return EnemyData.AttackComboCount; }
	UFUNCTION()
	float GetHPRatio() const { 
		if (EnemyData.HP == 0)
		{
			LOG_S(Error);
			return 0;
		}
		return curHP / EnemyData.HP; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
	FSLEnemyData EnemyData;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Type, Meta = (AllowPrivateAccess = true))
	uint8 ID;
	UPROPERTY(VisibleAnywhere, Category = Effect, Meta = (AllowPrivateAccess = true))
	UNiagaraSystem* DeadEffect;
	UPROPERTY(VisibleAnywhere, Category = UI, Meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HPBarWidget;
	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float curHP;
};
