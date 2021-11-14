// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "PingleStudioProjectile.h"
#include "TrialByTargets.h"
#include "Components/SphereComponent.h"
#include "Log.h"

DEFINE_LOG_CATEGORY(LogTargets)

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionProfileName(FName("Target"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATarget::OnOverlapBegin);
	CollisionComponent->SetEnableGravity(false);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(FName("NoCollision"));
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	
}

void ATarget::SetTargetSize(float Radius)
{
	// Radius can't be less than minimum 
	if (Radius <= MinimumRadius)
	{
		Radius = MinimumRadius;
	}
	SetActorScale3D(FVector(sqrt(Radius)));
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATarget::OnOverlapBegin_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && IsValid(OtherComp) && OtherActor != this)
	{
		if (Cast<APingleStudioProjectile>(OtherActor))
		{
			ATrialByTargets* TrialByTargets = Cast<ATrialByTargets>(GetOwner());
			if (!IsValid(TrialByTargets))
			{
				UE_LOG(LogTargets, Error, TEXT("ATrialByTargets is nullptr in ATarget::OnOverlapBegin()"));
				return;
			}
			TrialByTargets->TargetDestroyed(this);
			
			Destroy();
		}
	}
}
