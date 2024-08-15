// Fill out your copyright notice in the Description page of Project Settings.

#include "LSPlayer.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ALSPlayer::ALSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (TempMesh.Succeeded()) {

		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetUsingAbsoluteRotation(true);		// Don't want to rotate when character does.
	springArmComp->SetRelativeLocation(FVector(0, 0, 300));
	springArmComp->SetRelativeRotation(FRotator(0.0f, -50.0f, 0.0f));
	springArmComp->TargetArmLength = 700;

	topDownCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("topDownCamComp"));
	topDownCamComp->SetupAttachment(springArmComp);


}

// Called when the game starts or when spawned
void ALSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

