// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * The type of anim we should use.
 */
UENUM(BlueprintType,Blueprintable)
enum class EMovementAnimType : uint8
{
	ENONE UMETA(DisplayName = "None", Tooltip = "This is set when there isn't an advanced movement anim type."),
	ECLIMBUP UMETA(DisplayName = "Climb Up", Tooltip = "Change our anims to climbing up."),
	ECLIMBFRONT UMETA(DisplayName = "Climb Front", Tooltip = "Change our anims to climbing front."),
	ECLIMBLEFT UMETA(DisplayName = "Climb Left", Tooltip = "Change our anims to climbing left."),
	ECLIMBRIGHT UMETA(DisplayName = "Climb Right", Tooltip = "Change our anims to climbing right."),
	ECLIMBDOWN UMETA(DisplayName = "Climb Down", Tooltip = "Change our anims to climbing down."),
	ESLIDING UMETA(DisplayName = "Sliding", Tooltip = "Change our anims to sliding.")
};