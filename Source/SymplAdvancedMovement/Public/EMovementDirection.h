// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * The type of direction for movement.
 */
UENUM(BlueprintType,Blueprintable)
enum class EMovementDirection : uint8
{
	ENONE UMETA(DisplayName = "None", Tooltip = "No movement direction."),
	EFORWARD UMETA(DisplayName = "Forward", Tooltip = "Forward movement direction."),
	EBACKWARD UMETA(DisplayName = "Backward", Tooltip = "Backward movement direction."),
	ELEFT UMETA(DisplayName = "Left", Tooltip = "Left movement direction."),
	ERIGHT UMETA(DisplayName = "Right", Tooltip = "Right movement direction."),
	EUP UMETA(DisplayName = "Up", Tooltip = "Up movement direction."),
	EDOWN UMETA(DisplayName = "Down", Tooltip = "Down movement direction.")
};