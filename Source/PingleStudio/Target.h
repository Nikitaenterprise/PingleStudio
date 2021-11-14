// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

class USphereComponent;
UCLASS()
class PINGLESTUDIO_API ATarget : public AActor
{
	GENERATED_BODY()

public:

	// Collision sphere that will control collisions
	UPROPERTY(EditAnywhere, Category = "Target")
	USphereComponent* CollisionComponent = nullptr;
	// Mesh that visualize target
	UPROPERTY(EditAnywhere, Category = "Target")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	// Minimum target radius
	UPROPERTY(EditAnywhere, Category = "Target")
	float MinimumRadius = 10;


public:

	// Sets default values for this actor's properties
	ATarget();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target")
	void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetTargetSize(float Radius);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};