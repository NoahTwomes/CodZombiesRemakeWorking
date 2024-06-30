// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/MysteryBox.h"
#include "Zombies/Useables/WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"


AMysteryBox::AMysteryBox()
{
	MysteryBoxMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MysteryBoxMesh;

	TESTWEAPONMESHLOCATION = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponentTEST");
	TESTWEAPONMESHLOCATION->SetupAttachment(MysteryBoxMesh);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	CollisionMesh->SetupAttachment(MysteryBoxMesh);

	Cost = 950;

	bCanUseBox = true;

}

void AMysteryBox::BeginPlay()
{
	Super::BeginPlay();

	FVector BoxLocation = GetActorLocation();

	BottomLocation = FVector(BoxLocation.X += 25.0f, BoxLocation.Y += -2.0f, BoxLocation.Z += 25.0f);
	CurrentLocation = BottomLocation;
	TopLocation = FVector(BoxLocation.X += 26.0f, BoxLocation.Y += -2.0f, BoxLocation.Z += 130.0f);

	if (HasAuthority())
	{
		for (UClass* Class : WeaponClasses)
		{
			UE_LOG(LogTemp, Warning, TEXT("This Has Worked2"));

			if (AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(Class))
			{
				UE_LOG(LogTemp, Warning, TEXT("This Has Worked"));
				Weapon->SetActorLocation(TopLocation);
				FRotator Rotation = GetActorRotation();
				Rotation.Yaw += 90.0f;
				Weapon->SetActorRotation(Rotation);
				BoxWeaponArray.Add(Weapon);
			}
		}
	}
}

void AMysteryBox::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMysteryBox, bCanUseBox);
}

void AMysteryBox::AbleToGrabWeapon()
{
	GetWorld()->GetTimerManager().ClearTimer(TRaiseWeapon);
}

void AMysteryBox::RaiseWeapon()
{
	AWeaponBase* SelectedWeapon = BoxWeaponArray[0];
	if (SelectedWeapon == nullptr) return;

	CurrentLocation = UKismetMathLibrary::VInterpTo(CurrentLocation, TopLocation, GetWorld()->GetDeltaSeconds(), 20.0f);
	SelectedWeapon->SetActorLocation(CurrentLocation);


}

void AMysteryBox::LowerWeapon()
{
}

bool AMysteryBox::Multi_BoxUsed_Validate(uint8 RandomIndex)
{
	return true;
}

void AMysteryBox::Multi_BoxUsed_Implementation(uint8 RandomIndex)
{
	if (OpenAnimation)
		MysteryBoxMesh->PlayAnimation(OpenAnimation, false);

	FTimerHandle TGrabWeapon;
	GetWorld()->GetTimerManager().SetTimer(TGrabWeapon, this, &AMysteryBox::AbleToGrabWeapon, 4.0f, false);

	GetWorld()->GetTimerManager().SetTimer(TRaiseWeapon, this, &AMysteryBox::RaiseWeapon, 0.01f, true);
}

FString AMysteryBox::GetUIMessage(ACharacterBase* Player)
{
	if (bCanUseBox)
		return UIMessage;
	else
		return FString();
}

void AMysteryBox::Use(ACharacterBase* Player)
{
	if (HasAuthority() && Player)
	{
		bCanUseBox = false;
		uint8 RandomIndex = FMath::RandRange(0, BoxWeaponArray.Num());
		UE_LOG(LogTemp, Warning, TEXT("Weapon Count In Box: %d"), BoxWeaponArray.Num());
		Multi_BoxUsed(RandomIndex);
		if (OpenAnimation)
			MysteryBoxMesh->PlayAnimation(OpenAnimation, false);
	}
}
