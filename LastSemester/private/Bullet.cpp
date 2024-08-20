// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(5);
	RootComponent = collisionComp;

	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.1f));

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;

	//InitialLifeSpan = 2.0f;		// Bullet life span after spawn.
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// Bullet Object Life timer after spawned.
	FTimerHandle bulletLifeTimer;
	GetWorld()->GetTimerManager().SetTimer(bulletLifeTimer, this, &ABullet::BulletLife, 2.0f, false);

	/* Test Lambda Function 
	GetWorld()->GetTimerManager().SetTimer(bulletLifeTimer, FTimerDelegate::CreateLambda([this]()-> void {
		Destroy();
		}), 2.0f, false);
	*/

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::BulletLife() {
	Destroy();
}

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	if (PropertyChangedEvent.GetPropertyName() == TEXT("bulletSpeed")) {
		movementComp->InitialSpeed = bulletSpeed;
		movementComp->MaxSpeed = bulletSpeed;
	}
}