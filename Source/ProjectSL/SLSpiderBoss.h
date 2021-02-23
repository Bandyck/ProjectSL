// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SLSpiderBoss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnJumpAttackEndDelegate);

UCLASS()
class PROJECTSL_API ASLSpiderBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLSpiderBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void JumpAttack();

	float GetJumpAttackRange() const { return JumpAttackRange; }
	void SetJumpAttackRange(float newJumpAttackRange) { JumpAttackRange = newJumpAttackRange; }

	AActor* GetCircleIndicator() const { return CircleIndicator; }
	AActor* GetCircleProgressIndicator() const { return CircleProgressIndicator; }

private:
	AActor* MakeIndicator(FString bluePrintPath);
public :
	FOnJumpAttackEndDelegate JumpAttackEnd;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float JumpAttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	AActor* CircleIndicator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	AActor* CircleProgressIndicator;

private :
};
