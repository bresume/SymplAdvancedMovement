// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Animation/AnimSequenceBase.h"

#include "EMovementDirection.h"
#include "EAdvancedMovementMode.h"
#include "EMovementAnimType.h"
#include "FSymplMovementAnimation.h"
#include "FSymplMovementModeAnimation.h"

#include "FSymplMovementAnimations.generated.h"

/**
 * Animations for the sympl movement system.
 * These can be montages or anim sequences.
 * If you want to use anim sequences, set up your anim bp to work accordingly.
 * Otherwise, the system will automatically try to cast to montages and play them internally.
 * Tick events will go through the anim bp by default for performance and fluidity (i.e. sliding, blinking, etc...).
 * To change this, you will need to modify the code and recompile the plugin.
 */
USTRUCT(BlueprintType,Blueprintable)
struct SYMPLADVANCEDMOVEMENT_API FSymplMovementAnimations : public FTableRowBase
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
		TMap<TEnumAsByte<EMovementDirection>, FSymplMovementAnimation> DashAnims;

	/**
	 * Animations for blinking.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EMovementDirection>, FSymplMovementAnimation> BlinkAnims;

	/**
	 * Animations for rolling.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EMovementDirection>, FSymplMovementAnimation> RollAnims;

	/**
	 * Animations for wall run.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EMovementAnimType>, FSymplMovementAnimation> WallRunAnims;

	/**
	 * Animations for movement.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EAdvancedMovementMode>, FSymplMovementModeAnimation> MovementAnims;

	/**
	 * Animations for movement.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EAdvancedMovementMode>, FSymplMovementModeAnimation> ParachuteAnims;

	/**
	 * Animations for prone.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSymplMovementAnimation ProneAnim;

	/**
	 * Animations for sliding.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSymplMovementAnimation SlideAnim;

	/**
	 * Animations for double jumping.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSymplMovementAnimation DoubleJumpAnim;

	/**
	 * Animations for deploying a parachute.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSymplMovementAnimation ParachuteAnim;

	FSymplMovementAnimations()
	{
		Name = "";
		DashAnims.Empty(); 
		BlinkAnims.Empty();
		RollAnims.Empty();
		WallRunAnims.Empty();
		MovementAnims.Empty();
		ParachuteAnims.Empty();
		//ProneAnim = FSymplMovementAnimation();
		//SlideAnim = FSymplMovementAnimation();
		//DoubleJumpAnim = FSymplMovementAnimation();
		//
		ParachuteAnim = FSymplMovementAnimation();
	}

	FSymplMovementAnimations(FName InName,
		TMap<TEnumAsByte<EMovementDirection>, FSymplMovementAnimation> InDashAnims,
		TMap<TEnumAsByte<EMovementDirection>, FSymplMovementAnimation> InBlinkAnims,
		TMap<TEnumAsByte<EMovementDirection>, FSymplMovementAnimation> InRollAnims,
		TMap<TEnumAsByte<EMovementAnimType>, FSymplMovementAnimation> InWallRunAnims,
		TMap<TEnumAsByte<EAdvancedMovementMode>, FSymplMovementModeAnimation> InMovementAnims,
		TMap<TEnumAsByte<EAdvancedMovementMode>, FSymplMovementModeAnimation> InParachuteAnims,
		FSymplMovementAnimation InProneAnim,
		FSymplMovementAnimation InSlideAnim,
		FSymplMovementAnimation InDoubleJumpAnim,
		FSymplMovementAnimation InParachuteAnim
		)
	{
		Name = InName;
		DashAnims = InDashAnims;
		BlinkAnims = InBlinkAnims;
		RollAnims = InRollAnims;
		WallRunAnims = InWallRunAnims;
		MovementAnims = InMovementAnims;
		ParachuteAnims = InParachuteAnims;
		ProneAnim = InProneAnim;
		SlideAnim = InSlideAnim;
		DoubleJumpAnim = InDoubleJumpAnim;
		ParachuteAnim = InParachuteAnim;
	}
};
/**
uint32 GetTypeHash(const FSymplMovementAnimations& other)
{
	return FCrc::MemCrc32(&other, sizeof(FSymplMovementAnimations));
}
*/