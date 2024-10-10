// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Camera Objects")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera Objects")
	class UStaticMeshComponent* StartShootLocation;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MyAllWidgets;

	UUserWidget* CrosshairWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int BulletDistance;

	FTimerHandle CrouchTimerHandle;
	FTimerHandle SlideTimerHandle;
	FTimerHandle StartSlideTimerHandle;
	FTimerHandle JumpTimerHandle;

	FHitResult HitInfo;
	FCollisionQueryParams TraceParams;

	void Forward(float InputValue);
	void Right(float InputValue);

	void ToggleCrouch();

	void SmoothCrouch();

	void StartSprint();
	void StopSprint();

	void StartSlide();
	void Sliding();
	void StopSlide();

	void SetPhysicsSettingsZero();
	void SetPhysicsSettingsNormal();

	void StartBHOP();
	void BHOPING();
	void StopBHOP();

	bool bIsSprinting;
	bool bIsCrouching;
	bool bIsSliding;
	bool bIsJumping;

	float CapsuleHalfHeight;
	float CapsuleStandartHeight;

	float NormalGroundFriction;
	float SlideGroundFriction;
	float NormalBrakingDeceleration;
	float SlideBrakingDeceleration;

	void MouseX(float InputValue);
	void MouseY(float InputValue);

	void Fire();
};
