// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataTable.h"

#include "EAdvancedMovementMode.h"

#include "FSymplMovementSpeeds.generated.h"

/**
 * Movement speeds for the sympl advanced movement component.
 */
USTRUCT(BlueprintType, Blueprintable)
struct SYMPLADVANCEDMOVEMENT_API FSymplMovementSpeeds : public FTableRowBase
{

	GENERATED_BODY()

public:

	/**
	 * Name for JSON serialization and identification.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Name;

	/**
	 * The movement mode linked to this speed.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EAdvancedMovementMode> MovementMode;

	/**
	 * The new movement speed.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		double Speed;

	FSymplMovementSpeeds()
	{
		Name = "";
		MovementMode = EAdvancedMovementMode::ENONE;
		Speed = 0.f;
	}

	FSymplMovementSpeeds(FName InName, TEnumAsByte<EAdvancedMovementMode> InMovementMode, double InSpeed)
	{
		Name = InName;
		MovementMode = InMovementMode;
		Speed = InSpeed;
	}

};
/**
uint32 GetTypeHash(const FSymplMovementSpeeds& other)
{
	return FCrc::MemCrc32(&other, sizeof(FSymplMovementSpeeds));
}
*/