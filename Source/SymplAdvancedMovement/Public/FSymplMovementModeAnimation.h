// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

/**
 * Animation mapped to movement mode.
 * This is a cool way to wrap up all of your core Anim BP stuff to a data struct.
 * It doesn't have to be used, but it's here if you need it <3
*/
USTRUCT(BlueprintType, Blueprintable)
struct SYMPLADVANCEDMOVEMENT_API FSymplMovementModeAnimation : public FTableRowBase
{

	GENERATED_BODY()

public:

	/**
	 * Name for JSON serialization and identification.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Name;

	/**
	 * The linked movement mode.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EAdvancedMovementMode> MovementMode;

	/**
	 * Anim for walking.
	 * Should probably be a blendspace.
	 * You will need to cast to it in your anim bp.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* WalkAnim;

	/**
	 * Anim for jump start.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* JumpStartAnim;

	/**
	 * Anim for jump loop.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* JumpLoopAnim;

	/**
	 * Anim for jump end.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* JumpEndAnim;

	/**
	 * Anim for crouching.
	 * Should probably be a blend space.
	 * You will need to cast to it in your anim bp.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* CrouchAnim;

	/**
	 * Anim for sprinting.
	 * Should probably be a blend space.
	 * You will need to cast to it in your anim bp.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* SprintAnim;

	FSymplMovementModeAnimation()
	{
		Name = "";
		MovementMode = EAdvancedMovementMode::EWALK;
		WalkAnim = nullptr;
		JumpStartAnim = nullptr;
		JumpLoopAnim = nullptr;
		JumpEndAnim = nullptr;
		CrouchAnim = nullptr;
		SprintAnim = nullptr;
	}

	FSymplMovementModeAnimation(FName InName,
		TEnumAsByte<EAdvancedMovementMode> InMovementMode,
		UAnimSequenceBase* InWalkAnim,
		UAnimSequenceBase* InJumpStartAnim,
		UAnimSequenceBase* InJumpLoopAnim,
		UAnimSequenceBase* InJumpEndAnim,
		UAnimSequenceBase* InCrouchAnim,
		UAnimSequenceBase* InSprintAnim)
	{
		Name = InName;
		MovementMode = InMovementMode;
		WalkAnim = InWalkAnim;
		JumpStartAnim = InJumpStartAnim;
		JumpLoopAnim = InJumpLoopAnim;
		JumpEndAnim = InJumpEndAnim;
		CrouchAnim = InCrouchAnim;
		SprintAnim = InSprintAnim;
	}

};
/**
uint32 GetTypeHash(const FSymplMovementModeAnimation& other)
{
	return FCrc::MemCrc32(&other, sizeof(FSymplMovementModeAnimation));
}
*/