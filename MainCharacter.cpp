// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	StartShootLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Start Location"));
	StartShootLocation->SetupAttachment(Camera);

	BulletDistance = 5000;

	bIsCrouching = false;

	CapsuleStandartHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CapsuleHalfHeight = CapsuleStandartHeight * 0.5f;

	NormalGroundFriction = 8.0f;
	NormalBrakingDeceleration = 2048.0f;

	SlideGroundFriction = 0.0f;
	SlideBrakingDeceleration = 0.0f;

	TraceParams.AddIgnoredActor(this);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(GetCharacterMovement()->MaxWalkSpeed, 600.0f, 1000.0f);

	GetCharacterMovement()->GroundFriction = NormalGroundFriction;
	GetCharacterMovement()->BrakingDecelerationWalking = NormalBrakingDeceleration;

	if (MyAllWidgets) // For Crosshair
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), MyAllWidgets);

		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartBHOP);
	PlayerInputComponent->BindAction("Jump", IE_Repeat, this, &AMainCharacter::BHOPING);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopBHOP);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::StartSprint); // There is only IE_Pressed because I goaled to be controller simple and easy

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::ToggleCrouch); // Same reason

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::Right);

	PlayerInputComponent->BindAxis("MouseX", this, &AMainCharacter::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &AMainCharacter::MouseY);
}

void AMainCharacter::Forward(float InputValue)
{
	FVector CharacterForward = GetActorForwardVector();
	AddMovementInput(CharacterForward, InputValue);
	if (InputValue == 0.0f && bIsSprinting) { StopSprint(); }
}

void AMainCharacter::Right(float InputValue)
{
	FVector CharacterRight = GetActorRightVector();
	AddMovementInput(CharacterRight, InputValue);
}

void AMainCharacter::ToggleCrouch()
{
	bIsCrouching = !bIsCrouching;
	GetWorldTimerManager().SetTimer(CrouchTimerHandle, this, &AMainCharacter::SmoothCrouch, 0.01f, true);

	if (GetCharacterMovement()->MaxWalkSpeed >= 700.0f && bIsCrouching)
	{
		bIsSliding = true;
		StartSlide();
		GetWorldTimerManager().SetTimer(StartSlideTimerHandle, this, &AMainCharacter::StopSlide, 1.2f, false);
	}
}

void AMainCharacter::SmoothCrouch()
{
	float TargetCapsuleHeightValue = bIsCrouching ? CapsuleHalfHeight : CapsuleStandartHeight;
	float CurrentCapsuleHeightValue = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	float NewHeightValue = FMath::Lerp(CurrentCapsuleHeightValue, TargetCapsuleHeightValue, 0.05f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeightValue);

	if (FMath::Abs(CurrentCapsuleHeightValue - TargetCapsuleHeightValue) < 0.01f)
	{
		GetWorldTimerManager().ClearTimer(CrouchTimerHandle);
	}
}

void AMainCharacter::StartSlide()
{
	SetPhysicsSettingsZero();
	GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &AMainCharacter::Sliding, 0.01f, true);
}

void AMainCharacter::Sliding()
{
	if (GetInputAxisValue("MoveForward") >= 0.0f)
	{
		FVector CharacterForwardDirection = GetActorForwardVector();
		AddMovementInput(CharacterForwardDirection, 1.0f);
	}
	else { StopSlide(); }
}

void AMainCharacter::StopSlide()
{
	SetPhysicsSettingsNormal();
	GetWorldTimerManager().ClearTimer(SlideTimerHandle);
	bIsSliding = false;
}

void AMainCharacter::SetPhysicsSettingsZero()
{
	GetCharacterMovement()->GroundFriction = SlideGroundFriction;
	GetCharacterMovement()->BrakingDecelerationWalking = SlideBrakingDeceleration;
}

void AMainCharacter::SetPhysicsSettingsNormal()
{
	GetCharacterMovement()->GroundFriction = NormalGroundFriction;
	GetCharacterMovement()->BrakingDecelerationWalking = NormalBrakingDeceleration;
}

void AMainCharacter::MouseX(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void AMainCharacter::MouseY(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void AMainCharacter::Fire()
{
	FVector StartLocation = StartShootLocation->GetComponentLocation();
	FVector EndLocation = StartLocation + StartShootLocation->GetForwardVector() * BulletDistance;

	if (this->GetWorld()->LineTraceSingleByChannel(HitInfo, StartLocation, EndLocation , ECC_Visibility, TraceParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("Bir şey vurdun"));
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 5.0f, 0, 1.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.0f, 0, 1.0f);
	}
}

void AMainCharacter::StartSprint()
{
	if (!(bIsCrouching || bIsJumping))
	{
		GetCharacterMovement()->MaxWalkSpeed = 950.0f;
		bIsSprinting = true;
	}
}

void AMainCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bIsSprinting = false;
}

void AMainCharacter::StartBHOP()
{
	GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &ACharacter::Jump, 0.1f, true);
	bIsJumping = true;
	SetPhysicsSettingsZero();
}

void AMainCharacter::BHOPING()
{
	GetCharacterMovement()->MaxWalkSpeed += 5.0f;
}

void AMainCharacter::StopBHOP()
{
	GetWorldTimerManager().ClearTimer(JumpTimerHandle);
	bIsJumping = false;
	SetPhysicsSettingsNormal();
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}
