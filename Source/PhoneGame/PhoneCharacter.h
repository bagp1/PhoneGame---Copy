// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "PhoneCharacter.generated.h"


UCLASS(meta = (BlueprintSpawnableComponent), Blueprintable)
class PHONEGAME_API APhoneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APhoneCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//float getFloat(char[] , int);
	UFUNCTION(Blueprintpure, Category = GAME, meta = (WorldContext = "Phone Connection")) FVector getGyroVector(); // Category = "connection"
	
};
