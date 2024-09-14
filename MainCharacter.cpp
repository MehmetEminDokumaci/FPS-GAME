// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	BulletDistance = 100;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MyAllWidgets)
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Repeat, this, &AMainCharacter::Fire);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::StopRunning);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::Right);

	PlayerInputComponent->BindAxis("MouseX", this, &AMainCharacter::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &AMainCharacter::MouseY);
}

void AMainCharacter::Forward(float InputValue)
{
	FVector CharacterForward = GetActorForwardVector();
	AddMovementInput(CharacterForward, InputValue);
}

void AMainCharacter::Right(float InputValue)
{
	FVector CharacterRight = GetActorRightVector();
	AddMovementInput(CharacterRight, InputValue);
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
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	FHitResult HitInfo;
	UWorld* World = this->GetWorld();
	FVector StartFVector = Camera->GetComponentLocation();
	FVector EndFVector = StartFVector + Camera->GetForwardVector() * BulletDistance * 50;

	if (World->LineTraceSingleByChannel(HitInfo, StartFVector, EndFVector, ECC_WorldStatic, TraceParams))
	{
		DrawDebugLine(GetWorld(), StartFVector, EndFVector, FColor::Red, false, 1.0f, 0, 5.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), StartFVector, EndFVector, FColor::Green, false, 1.0f, 0, 5.0f);
	}
}

void AMainCharacter::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AMainCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}