// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterBase.generated.h"


class UCameraComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int32, MagazineAmmo, int32, TotalAmmo);


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
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	TSubclassOf<class AWeaponBase> SecondWeaponClass;
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	TSubclassOf<class AWeaponBase> ThirdWeaponClass;

	UPROPERTY(ReplicatedUsing = OnRep_AttachWeapon)
	class AWeaponBase* CurrentWeapon;
	//UPROPERTY(Replicated)
	class AWeaponBase* PreviousWeapon;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SwtichWeapon(class AWeaponBase* NewWeapon, int32 NewWeaponIndex);
	bool Server_SwtichWeapon_Validate(class AWeaponBase* NewWeapon, int32 NewWeaponIndex);
	void Server_SwtichWeapon_Implementation(class AWeaponBase* NewWeapon, int32 NewWeaponIndex);

	UFUNCTION()
	void OnRep_AttachWeapon();

	UPROPERTY(Replicated)
	int32 WeaponIndex;
	UPROPERTY(Replicated)
	 TArray<AWeaponBase*> WeaponArray;
	 
	 UFUNCTION(BlueprintCallable)
	 void RefreshAmmoWidget();

	//set to replicate skip owner
	UPROPERTY(Replicated)
	bool bIsAiming;

	TArray<AWeaponBase*>* GetWeaponArray() { return &WeaponArray; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool WantsToAim);
	bool Server_SetAiming_Validate(bool WantsToAim);
	void Server_SetAiming_Implementation(bool WantsToAim);


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

	UPROPERTY()
	bool upgraded;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite)
	float XP;

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	bool HasPerk1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WalkSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RunSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxStamina;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentStamina;

	UPROPERTY()
	bool StaminaGain;

	UPROPERTY()
	bool StaminaReduce;

	UPROPERTY()
	bool bIsSprinting;

	UFUNCTION(BlueprintCallable)
	class AWeaponBase* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiDie();
	bool MultiDie_Validate();
	void MultiDie_Implementation();

public:
	void GivePlayerWeapon(class AWeaponBase* Weapon);

protected:
	UPROPERTY(BlueprintAssignable)
	FInteractChanged OnInteractChanged;

	UPROPERTY(BlueprintAssignable)
	FAmmoChanged OnAmmoChanged;

	FTimerHandle TInteractTimerHandle;
	class AInteractableBase* Interactable;

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange;

	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void Walk();

	FTimerHandle StaminaGainTimerHandle;
	FTimerHandle StaminaReduceTimerHandle;


	void StaminaGainFunction();
	void StaminaReduceFunction();
	void TimerHandles();


protected:
	void Interact();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(class AInteractableBase* InteractingObject);
	bool Server_Interact_Validate(class AInteractableBase* InteractingObject);
	void Server_Interact_Implementation(class AInteractableBase* InteractingObject);
	void SetInteractableObject();


	virtual void OnFire();
	virtual void OnStopFire();
	void OnReload();
	 virtual void OnAimingStart();
	 virtual void OnAimingEnd();
	 void SwitchNextWeapon();
	 void SwitchPreviousWeapon();



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
