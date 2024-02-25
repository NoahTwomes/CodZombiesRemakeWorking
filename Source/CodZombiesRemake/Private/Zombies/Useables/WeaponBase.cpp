// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/WeaponBase.h"
#include "Player/CharacterBase.h"


#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase()
{

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = WeaponMesh;

	SetReplicates(true);


	WeaponName = "Default";
	WeaponMaxAmmo = 255;
	MagazineMaxAmmo = 30;
 
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponBase, CurrentTotalAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeaponBase, CurrentMagazineAmmo, COND_OwnerOnly);
}

TArray<FHitResult> AWeaponBase::PerformLineTrace(ACharacterBase* ShootingPlayer)
{

	FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
	FVector Rot = WeaponMesh->GetSocketRotation(FName("muzzleSocket")).Vector();
	FVector End = Start + Rot * 2000.0f;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(ShootingPlayer);

	FCollisionResponseParams CollisionResponse;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);
	GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);
	return HitResults;
}

TArray<FHitResult> AWeaponBase::PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	
	FVector End = MuzzleLocation + MuzzleRotation.Vector() * 2000.0f;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetOwner())
	CollisionParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams CollisionResponse;

	DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f, 0, 3.0f);
	GetWorld()->LineTraceMultiByChannel(OUT HitResults, MuzzleLocation, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);
	return HitResults;
}

bool AWeaponBase::Server_Fire_Validate(const TArray<FHitResult>& HitResults)
{
	return true;
}

void AWeaponBase::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	if (CurrentMagazineAmmo > 0)
		--CurrentMagazineAmmo;
	UE_LOG(LogTemp, Warning, TEXT("ServerAmmo: %d"), CurrentMagazineAmmo);
}

bool AWeaponBase::Fire(ACharacterBase* ShootingPlayer)
{
	if (CurrentMagazineAmmo > 0)
	--CurrentMagazineAmmo;
	UE_LOG(LogTemp, Warning, TEXT("ClientAmmo: %d"), CurrentMagazineAmmo);
	return true;
}

FWeaponDamage AWeaponBase::GetWeaponDamage()
{
	return WeaponDamage;
}

bool AWeaponBase::Reload()
{
	return true;
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	
	return { CurrentMagazineAmmo, CurrentTotalAmmo };
}

UAnimMontage* AWeaponBase::GetFireAnimMontage()
{
	return FPSArmsFireMontage;
}

TEnumAsByte<EWeaponID> AWeaponBase::GetWeaponID()
{
	return WeaponID;
}


