// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * The movement mode type for advanced movement.
 */
UENUM(BlueprintType,Blueprintable)
enum class EAdvancedMovementMode : uint8
{
	ENONE UMETA(DisplayName = "None", Tooltip = "Set movement mode to none."),
	EFLY UMETA(DisplayName = "Flying", Tooltip = "Set movement mode to flying."),
	ECROUCH UMETA(DisplayName = "Crouching", Tooltip = "Set movement mode to crouch."),
	EWALK UMETA(DisplayName = "Walking", Tooltip = "Set movement mode to walk."),
	ESPRINT UMETA(DisplayName = "Sprinting", Tooltip = "Set movement mode to sprint."),
	EPRONE UMETA(DisplayName = "Prone", Tooltip = "Set movement mode to prone."),
	ESWIMMING UMETA(DisplayName = "Swimming", Tooltip = "Set movement mode to swimming."),
	ECLIMBING UMETA(DisplayName = "Climbing", Tooltip = "Set movement mode to climbing."),
	ESLIDE UMETA(DisplayName = "Sliding", Tooltip = "Set movement mode to sliding."),
	EJUMP UMETA(DisplayName = "Jumping", Tooltip = "Set movement mode to jumping."),
	EPARACHUTE UMETA(DisplayName = "Parachute", Tooltip = "Set movement mode to parachuting"),
	ESUPERFALL UMETA(DisplayName = "Super Fall", Tooltip = "Set movement mode to super fall. This is a state like a battle royale game when you drop from the plane."),
	EDASH UMETA(DisplayName = "Dash", Tooltip = "Set the movement mode to dash."),
	EBLINK UMETA(DisplayName = "Blink", Tooltip = "Set the movement mode to blinking."),
	EROLL UMETA(DisplayName = "Roll", Tooltip = "Set the movement mode to rolling."),
	EHOVER UMETA(DisplayName = "Hover", Tooltip = "Set the movement mode to hovering."),
	EZEROG UMETA(DisplayName = "Zero Gravity", Tooltip = "Set the movement mode to zero gravity."),
	EJETPACK UMETA(DisplayName = "Jetpack", Tooltip = "Set the movement mode to jetpack.")
};