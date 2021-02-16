// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SLEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
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

	void Attack();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FOnAttackEndDelegate OnAttackEnd;

	float GetAttackRange() { return AttackRange; }
	void SetAttackRange(float attackRange) { AttackRange = attackRange; }

	float GetAttackRadius() { return AttackRadius; }
	void SetAttackRadius(float attackRadius) { AttackRadius = attackRadius; }
protected:
	float AttackRange;
	float AttackRadius;
private:
};
