// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SymplAdvancedMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USymplAdvancedMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SYMPLADVANCEDMOVEMENT_API ISymplAdvancedMovementInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Determine if the player is falling.
	 * Used for actors, pawns and characters.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AdvancedMovement")
		bool SymplIsFalling();

	/**
	 * Get the owner's movement speed.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AdvancedMovement")
		float GetOwnerSpeed();

	/**
	 * Get the owner's movement speed.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AdvancedMovement")
		FVector GetOwnerMovementDirection();

};
