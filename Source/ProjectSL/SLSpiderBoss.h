// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SLSpiderBoss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnJumpAttackEndDelegate);

UCLASS()
class PROJECTSL_API UJumpAttackCameraShake : public UCameraShake
{
	GENERATED_BODY()

public :
	UJumpAttackCameraShake()
	{
		OscillationDuration = 0.3f;
		OscillationBlendInTime = 0.15f;
		OscillationBlendOutTime = 0.15f;

		RotOscillation.Pitch.Amplitude = 1.0f;
		RotOscillation.Pitch.Frequency = 25.0f;

		RotOscillation.Yaw.Amplitude = -1.0f;
		RotOscillation.Yaw.Frequency = 25.0f;
	}
};

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
	UParticleSystem* GetRockDropParticle() const { return RockDropParticle; }
	UParticleSystem* GetLandDestroyParticle() const { return LandDestroyParticle; }
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UParticleSystem* RockDropParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UParticleSystem* LandDestroyParticle;
	UPROPERTY(EditDefaultsOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShake> JumpAttackCameraShakeClass = UJumpAttackCameraShake::StaticClass();
private :
};
