// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "SLBossProjectile.generated.h"

UCLASS(Blueprintable)
class PROJECTSL_API ASLBossProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLBossProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float ProjectileSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true));
	float ProjectileDamage;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* ProjectileParticle = nullptr;*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ABPawnComponent")
	class UCapsuleComponent* Body = nullptr;
	FVector ProjectileDir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* ProjectileParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* HitParticle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void SetProjectileDir(FVector dir) { ProjectileDir = dir; }
	float GetHeight() { return Body->GetScaledCapsuleHalfHeight(); }
protected:
	UFUNCTION()
	void TornadoBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
