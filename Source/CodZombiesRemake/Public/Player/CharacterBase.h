// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterBase.generated.h"


class UCameraComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);


UCLASS()
class CODZOMBIESREMAKE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	TSubclassOf<class AWeaponBase> StartingWeaponClass;

	UPROPERTY(ReplicatedUsing = OnRep_AttachWeapon)
	class AWeaponBase* CurrentWeapon;
	UFUNCTION()
	void OnRep_AttachWeapon();

	int32 WeaponIndex;
	TArray<AWeaponBase*> WeaponArray;

	//set to replicate skip owner
	bool bIsAiming;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;


public:
	// Sets default values for this character's properties
	ACharacterBase();

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsAiming();

	UFUNCTION(BlueprintCallable)
	class AWeaponBase* GetCurrentWeapon();

protected:
	UPROPERTY(BlueprintAssignable)
	FInteractChanged OnInteractChanged;



	FTimerHandle TInteractTimerHandle;
	class AInteractableBase* Interactable;

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange;



protected:
	void Interact();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(class AInteractableBase* InteractingObject);
	bool Server_Interact_Validate(class AInteractableBase* InteractingObject);
	void Server_Interact_Implementation(class AInteractableBase* InteractingObject);
	void SetInteractableObject();



	virtual void OnFire();

	 virtual void OnAimingStart();
	 virtual void OnAimingEnd();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
