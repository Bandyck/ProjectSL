// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "SLEnemy.generated.h"

USTRUCT(BlueprintType)
struct FSLEnemyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FSLEnemyData() : ID(0), Name("NoName"), HP(0), AttackPower(0), AttackRange(0), RecognitionRange(0), WalkSpeed(0)
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
	float RecognitionRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyData)
	float WalkSpeed;
};


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FOnAttackEndDelegate OnAttackEnd;

	float GetAttackRange() const { return EnemyData.AttackRange; }
	void SetAttackRange(float attackRange) { EnemyData.AttackRange = attackRange; }

	float GetAttackRadius() const { return EnemyData.AttackRange; }
	void SetAttackRadius(float attackRadius) { EnemyData.AttackRange = attackRadius; }

	 float GetAttackPower() const { return EnemyData.AttackPower;  }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
	FSLEnemyData EnemyData;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Type, Meta = (AllowPrivateAccess = true))
	uint8 ID;
};
