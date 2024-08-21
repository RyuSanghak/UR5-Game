// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "LSPlayer.generated.h"


UCLASS()
class LASTSEMESTER_API ALSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// Setup Player TopDown Camera
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* springArmComp;
	
	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* topDownCamComp;

	//Setup Player Move control
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imc_PlayerController;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_PlayerController; // move
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Jump;			// jump

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float playerSpeed = 600;

	FVector direction;

	void Move(const struct FInputActionValue& inputValue);
	void PlayerMove();
	void inputJump(const struct FInputActionValue& inputValue);

	// Player looks in mouse direction.
	APlayerController* PlayerController;
	FVector mouseLocation;
	FVector mouseDirection;
	
	void lookAtMouse();

	// Setup player Gun Mesh
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, Category = BulletMaker)
	TSubclassOf<class ABullet> bulletMaker;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_Fire;
	void inputFire(const struct FInputActionValue& inputValue);

};
