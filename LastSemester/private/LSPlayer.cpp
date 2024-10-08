// Fill out your copyright notice in the Description page of Project Settings.

#include "LSPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Bullet.h"

// Sets default values
ALSPlayer::ALSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (TempMesh.Succeeded()) {

		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetUsingAbsoluteRotation(true);		// Don't want to rotate when character does.
	springArmComp->SetRelativeLocation(FVector(-200, 0, 300));
	springArmComp->SetRelativeRotation(FRotator(0.0f, -50.0f, 0.0f));
	springArmComp->TargetArmLength = 700;

	topDownCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("topDownCamComp"));
	topDownCamComp->SetupAttachment(springArmComp);
	topDownCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	// GunMesh Component
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded()) {
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	// indicator Mesh Component
	directionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("indicatorMeshComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempIndicatorMesh(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/indicator.indicator'"));
	if (TempIndicatorMesh.Succeeded()) {
		directionIndicator->SetupAttachment(RootComponent);
		directionIndicator->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		directionIndicator->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	

}

// Called when the game starts or when spawned
void ALSPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(Controller);
	if (pc) {

		pc->bShowMouseCursor = false; // show up mouse cursor

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
	PlayerMove();
	// player to look in the direction of the mouse
	lookAtMouse();
	// update directionIndicator's position to setup in front of character.
	updateDirectionIndicator();

}

// Called to bind functionality to input
void ALSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput) {
		PlayerInput->BindAction(ia_PlayerController, ETriggerEvent::Triggered, this, &ALSPlayer::Move);
		PlayerInput->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &ALSPlayer::inputJump);
		PlayerInput->BindAction(ia_Fire, ETriggerEvent::Triggered, this, &ALSPlayer::inputFire);
	}

}

void ALSPlayer::Move(const struct FInputActionValue& inputValue) {
	FVector2D value = inputValue.Get<FVector2D>();

	direction.X = value.X; // W,S value (move Forward and Backward)
	direction.Y = value.Y; // A, D value (move left and right)
}

void ALSPlayer::PlayerMove() {
	/*
	FVector CurrentPos = GetActorLocation();
	FVector Displacement = direction * playerSpeed * DeltaTime; // Displacement = Velocity * Time
	FVector MovedPos = CurrentPos + Displacement;
	SetActorLocation(MovedPos);
	*/
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction.X = 0;
	direction.Y = 0;
	direction.Z = 0;
}

void ALSPlayer::inputJump(const struct FInputActionValue& inputValue) {
	Jump();
}

void ALSPlayer::inputFire(const struct FInputActionValue& inputValue) {
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletMaker, firePosition);
}

void ALSPlayer::lookAtMouse() {

	auto pc = Cast<APlayerController>(Controller);
	if (pc) {

		FHitResult Hit;

		if (pc->DeprojectMousePositionToWorld(mouseLocation, mouseDirection)) {
			FVector characterLocation = GetActorLocation();
			FVector LookAtTarget = mouseLocation + (mouseDirection * 1000.0f);

			FRotator LookAtRotation = FRotationMatrix::MakeFromX(LookAtTarget - characterLocation).Rotator();

			//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, LookAtRotation.ToString());
			 
			// only change the Yaw value
			LookAtRotation.Pitch = 0.0f;
			LookAtRotation.Roll = 0.0f;
			LookAtRotation.Yaw += -90.0f;

			// only change the Rotation value of player mesh component.
			GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), LookAtRotation);
		}
	}
}

void ALSPlayer::updateDirectionIndicator() {

	FRotator CharacterRotation = GetMesh()->GetRelativeRotation();
	FVector CharacterLocation = GetMesh()->GetRelativeLocation();
	FRotator newRotation = FRotator(CharacterRotation.Pitch + -90.0f, CharacterRotation.Yaw + 90.0f, CharacterRotation.Roll);
	directionIndicator->SetRelativeRotation(newRotation);

}
