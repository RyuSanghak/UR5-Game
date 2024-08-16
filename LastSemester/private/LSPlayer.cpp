// Fill out your copyright notice in the Description page of Project Settings.

#include "LSPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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
	topDownCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;


}

// Called when the game starts or when spawned
void ALSPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(Controller);
	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(imc_PlayerController, 0);
		}
	}
	
}

// Called every frame
void ALSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update player moved per frame
	FVector CurrentPos = GetActorLocation();
	FVector Displacement = direction * playerSpeed * DeltaTime; // Displacement = Velocity * Time
	FVector MovedPos = CurrentPos + Displacement;
	SetActorLocation(MovedPos);
	direction.X = 0;
	direction.Y = 0;
	direction.Z = 0;

}

// Called to bind functionality to input
void ALSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput) {
		PlayerInput->BindAction(ia_PlayerController, ETriggerEvent::Triggered, this, &ALSPlayer::Move);
		PlayerInput->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &ALSPlayer::inputJump);
	}

}

void ALSPlayer::Move(const struct FInputActionValue& inputValue) {
	FVector2D value = inputValue.Get<FVector2D>();

	direction.X = value.X; // W,S value (move Forward and Backward)
	direction.Y = value.Y; // A, D value (move left and right)
}

void ALSPlayer::inputJump(const struct FInputActionValue& inputValue) {
	Jump();
}

