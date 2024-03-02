// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterBase.h"
#include "Zombies/ZombieBase.h"
#include "Zombies/Useables/WeaponBase.h"


#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Zombies/Useables/InteractableBase.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"





// Sets default values
ACharacterBase::ACharacterBase()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Interactable = nullptr;
	InteractionRange = 200.0f;


	WeaponIndex = 0;

	bIsAiming = false;

}

bool ACharacterBase::GetIsAiming()
{
	return bIsAiming;
}

AWeaponBase* ACharacterBase::GetCurrentWeapon()
{
	return CurrentWeapon;
}


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	



	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &ACharacterBase::SetInteractableObject, 0.2f, true);

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StartingWeaponClass, SpawnParams);
		if (CurrentWeapon)
		{


			WeaponArray.Add(CurrentWeapon);

			OnRep_AttachWeapon();
			UE_LOG(LogTemp, Warning, TEXT("THIS WORKED"));

		}
	}
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, CurrentWeapon);
	DOREPLIFETIME_CONDITION(ACharacterBase, bIsAiming, COND_SkipOwner);
}

void ACharacterBase::OnRep_AttachWeapon()
{
	if (CurrentWeapon)
	{

		if (true || IsLocallyControlled())
		{

			CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_weaponSocket"));
			UE_LOG(LogTemp, Warning, TEXT("SPAWNED AND ATTEMPTED TO ATTACH WEAPON TO HAND LOCALLY"));
		}
		else
		{

			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_weaponSocket"));
			UE_LOG(LogTemp, Warning, TEXT("SPAWNED AND ATTEMPTED TO ATTACH WEAPON TO MESH"));

		}
	}
}





// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);

		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::Interact);

		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::OnFire);
		PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::OnStopFire);

		PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACharacterBase::OnAimingStart);
		PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACharacterBase::OnAimingEnd);

		PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACharacterBase::OnReload);
	}

}

void  ACharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void  ACharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void  ACharacterBase::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool  ACharacterBase::GetHasRifle()
{
	return bHasRifle;
}



void ACharacterBase::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACTING"));

	if (Interactable)
	{
		if (HasAuthority())
			Interactable->Use(this);
		else
			Server_Interact(Interactable);
	}
}

bool ACharacterBase::Server_Interact_Validate(AInteractableBase* InteractingObject)
{
	return true;
}

void ACharacterBase::Server_Interact_Implementation(AInteractableBase* InteractingObject)
{
	float Distance = GetDistanceTo(InteractingObject);
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
	if (Distance < InteractionRange + 30.0f)
	{
		InteractingObject->Use(this);
	}
}

void ACharacterBase::SetInteractableObject()
{
	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	FVector End = Start + Rot * InteractionRange;

	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams);

	AInteractableBase * Temp = Cast<AInteractableBase>(HitResult.GetActor());
	if (Interactable == nullptr && Temp)
	{
		UE_LOG(LogTemp, Warning, TEXT("IS NOW A VALID PTR"));
		Interactable = Temp;
		OnInteractChanged.Broadcast(Interactable->GetUIMessage());
	
	}

	else if (Interactable && Temp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IS NOW A NULL PTR"));
		Interactable = nullptr;
		OnInteractChanged.Broadcast(FString());
	}



	if (Temp)
	{
		Interactable = Temp;
		
	}
	else
	{
		Interactable = nullptr;
	
	}
}




void ACharacterBase::OnFire()
{
	
	if (CurrentWeapon)
	{
		if (CurrentWeapon->Fire(this))
		{
			if (UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance())
			{
				if (UAnimMontage* FireMontage = CurrentWeapon->GetFireAnimMontage())
				{
					UE_LOG(LogTemp, Warning, TEXT("PLAYING FIRE MONTAGE"));
					AnimInstance->Montage_Play(FireMontage);
					if (bIsAiming)
						AnimInstance->Montage_JumpToSection(FName("FireADS"), FireMontage);
					else
						AnimInstance->Montage_JumpToSection(FName("FireHip"), FireMontage);
				}
			}
		}

	}

	
}

void ACharacterBase::OnStopFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StopFiring();
}

void ACharacterBase::OnReload()
{
	if (CurrentWeapon && CurrentWeapon->Reload())
	{
		
		
		if (UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance())
		{
			if (UAnimMontage* FireMontage = CurrentWeapon->GetFireAnimMontage())
			{
				AnimInstance->Montage_Play(FireMontage);
				AnimInstance->Montage_JumpToSection(FName("Reload"), FireMontage);

			}
		}
		
	}
}

bool ACharacterBase::Server_SetAiming_Validate(bool WantsToAim)
{
	return true;
}

void ACharacterBase::Server_SetAiming_Implementation(bool WantsToAim)
{
	bIsAiming = WantsToAim;
}

void ACharacterBase::OnAimingStart()
{
	bIsAiming = true;
	if (!HasAuthority())
	Server_SetAiming(bIsAiming);
}

void ACharacterBase::OnAimingEnd()
{
	bIsAiming = false;
	if (!HasAuthority())
	Server_SetAiming(bIsAiming);
}



