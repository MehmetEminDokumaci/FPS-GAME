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

	UPROPERTY(EditAnywhere, Category = "Object")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MyAllWidgets;

	UUserWidget* CrosshairWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int BulletDistance;

	void Forward(float InputValue);
	void Right(float InputValue);

	void MouseX(float InputValue);
	void MouseY(float InputValue);

	void Fire();

	void StartRunning();
	void StopRunning();
};