// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "GameFramework/Actor.h"
#include "SLBossProjectile.generated.h"

UCLASS()
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
	class UParticleSystem* ProjectileParticle = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ABPawnComponent")
	class UCapsuleComponent* Body = nullptr;
	FVector ProjectileDir;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetProjectileDir(FVector dir) { ProjectileDir = dir; }
};
