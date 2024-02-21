// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/WeaponBase.h"
#include "Player/CharacterBase.h"


#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"

// Sets default values
AWeaponBase::AWeaponBase()
{

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = WeaponMesh;

	SetReplicates(true);


	WeaponName = "Default";
	BaseDamage = 100;
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
	GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
	return HitResults;
}

TArray<FHitResult> AWeaponBase::Fire(ACharacterBase* ShootingPlayer)
{
	return TArray<FHitResult>();
}

void AWeaponBase::Reload()
{

}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	
	return { CurrentMagazineAmmo, CurrentTotalAmmo };
}

UAnimMontage* AWeaponBase::GetFireAnimMontage()
{
	return FPSArmsFireMontage;
}


