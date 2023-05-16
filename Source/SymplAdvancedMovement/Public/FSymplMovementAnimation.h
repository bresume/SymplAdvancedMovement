// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FSymplMovementAnimation.generated.h"

/**
 * Root animation wrapper for the animation that is used in FSymplMovementAnimations.
 */
USTRUCT(BlueprintType, Blueprintable)
struct SYMPLADVANCEDMOVEMENT_API FSymplMovementAnimation : public FTableRowBase
{

	GENERATED_BODY()

public:

	/**
	 * Name for JSON serialization and identification.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Name;

	/**
	 * Animations for dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* Anim;

	/**
	 * Animations for dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseAnimInstance;

	/**
	 * Animations for dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayRate;

	/**
	 * Animations for dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartPosition;

	/**
	 * Animations for dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStopMontages;

	/**
	 * Animations for dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName StartSectionName;

	FSymplMovementAnimation()
	{
		Name = "";
		Anim = nullptr;
		bUseAnimInstance = true;
		PlayRate = 1.f;
		StartPosition = 0.f;
		bStopMontages = true;
		StartSectionName = "";
	}

	FSymplMovementAnimation(FName InName,
		UAnimSequenceBase* InAnim,
		bool InUseAnimInstance,
		float InPlayRate,
		float InStartPosition,
		bool InStopMontages,
		FName InStartSectionName)
	{
		Name = InName;
		Anim = InAnim;
		bUseAnimInstance = InUseAnimInstance;
		PlayRate = InPlayRate;
		StartPosition = InStartPosition;
		bStopMontages = InStopMontages;
		StartSectionName = InStartSectionName;
	}
};
/**
uint32 GetTypeHash(const FSymplMovementAnimation& other)
{
	return FCrc::MemCrc32(&other, sizeof(FSymplMovementAnimation));
}
*/