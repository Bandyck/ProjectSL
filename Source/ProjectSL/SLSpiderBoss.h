// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "GameFramework/Character.h"
#include "SLSpiderBoss.generated.h"

class USLEnemyWidget;

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


DECLARE_MULTICAST_DELEGATE(FOnJumpAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBaseAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void JumpAttack();
	void BaseAttack();
	void RangeAttack();
	void BreathAttack();
	void CameraShake();
	
	float GetJumpAttackRange() const { return JumpAttackRange; }
	void SetJumpAttackRange(float newJumpAttackRange) { JumpAttackRange = newJumpAttackRange; }

	AActor* GetCircleIndicator() const { return CircleIndicator; }
	AActor* GetCircleProgressIndicator() const { return CircleProgressIndicator; }
	UParticleSystem* GetRockDropParticle() const { return RockDropParticle; }
	UParticleSystem* GetLandDestroyParticle() const { return LandDestroyParticle; }
	UBlueprint* GetTornadoProjectileBP() const { return  TornadoActor; }
	UClass* GetTornadoProjectileClass() const { return TornadoActor2; }
	UClass* GetBreathAttackClass() const { return BreathAttack2; }
	UBlueprint* GetTorText() const { return TornadoActor3; }
	UBlueprint* GetBreathAttackBP() const { return BreathActor; }
	float GetHPRatio() const { return CurrentHP / MaxHP; }
private:
	AActor* MakeIndicator(FString bluePrintPath);
public :
	FOnJumpAttackEndDelegate JumpAttackEnd;
	FOnBaseAttackEndDelegate BaseAttackEnd;
	FOnHPChangedDelegate HPChanged;
protected:
	class UClass* BossWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	UUserWidget* BossWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default, Meta = (AllowPrivateAccess = true))
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float JumpAttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	AActor* CircleIndicator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	AActor* CircleProgressIndicator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UBlueprint* TornadoActor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UClass* TornadoActor2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UClass* BreathAttack2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UBlueprint* TornadoActor3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UBlueprint* BreathActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UParticleSystem* RockDropParticle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UParticleSystem* LandDestroyParticle;
	UPROPERTY(EditDefaultsOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShake> JumpAttackCameraShakeClass = UJumpAttackCameraShake::StaticClass();

	float CurrentHP;
private :
};
