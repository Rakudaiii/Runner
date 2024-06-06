// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f,0.0f,60.0f));
	CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 450.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    GetCharacterMovement()->SetWalkableFloorAngle(60);
	
	this->bDead = false;

	LaneWidth = 200.0f;
	CurrentLane = 1;

}



// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	MoveForward(1.0f);
	

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMainCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AMainCharacter::MoveRight);

	
}

void AMainCharacter::MoveForward(float Axis)
{
	if ((Controller != NULL) && (Axis != 0.0f) && (bDead != true))
	{
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void AMainCharacter::MoveRight(float Axis)
{
	if ((Controller != NULL) && (Axis != 0.0f) && (bDead != true))
	{
		const FVector Direction = FVector::ForwardVector;
		AddMovementInput(Direction, Axis);
	}
}

void AMainCharacter::MoveLeft()
{
	if (CurrentLane > 0)
	{
		CurrentLane--;
		UpdateLanePosition();
	}
}

void AMainCharacter::MoveRight()
{
	if (CurrentLane < 2)
	{
		CurrentLane++;
		UpdateLanePosition();
	}
}

void AMainCharacter::UpdateLanePosition()
{
	float TargetX = (CurrentLane - 1) * LaneWidth;
	FVector NewLocation = GetActorLocation();
	NewLocation.X = TargetX;
	SetActorLocation(NewLocation);
	
}


