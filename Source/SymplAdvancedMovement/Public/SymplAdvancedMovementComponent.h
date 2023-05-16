// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameframework/Character.h"
#include "Gameframework/Pawn.h"
#include "Engine/DataTable.h"

#include "EMovementAnimType.h"
#include "EAdvancedMovementMode.h"
#include "EMovementDirection.h"
#include "FSymplMovementSpeeds.h"
#include "FSymplMovementAnimations.h"
#include "FSymplMovementAnimation.h"
#include "FSymplMovementModeAnimation.h"

#include "SymplAdvancedMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOwnerJump, USymplAdvancedMovementComponent*, Component, bool, bCustomJump, bool, bDoubleJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAutoRunStateUpdate, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidClimb, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidCrouch, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidSlide , USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidSprint, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedSpeedsUpdate, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentSpeedUpdate, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementModeUpdate, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidProne, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidHover, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidDash, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidBlink, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidRoll, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidParachute, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidZeroG, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDidJetpack, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJetpackFuelUpdate, USymplAdvancedMovementComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationUpdate, USymplAdvancedMovementComponent*, Component);

/**
 * An advanced component for managing an actor's movement capabilities.
 * It is compatible with Characters, Pawns and Actors, but some functions might require custom input.
 * All features are supported by Character.
 * Current Feature List:
 *	Prone
 *	Crouch
 *	Jump/Double Jump
 *	Wall Running/Climbing
 *	Sliding
 *	Custom movement modes/speeds
 *	Auto Run
 *	Sprint
 *  Dash
 *  Blinking
 *  Roll
 *  Hover
 *  Slope based speed adjustments
 *  Parachute
 *  Zero G Movement
 *	Jetpack
 *  Network replication.
 * TODO::
 *  LEDGE CLIMBING, LADDER CLIMBING, Ragdoll with Get up, VAULT/MANTLE, WALLHANG ADD SUPPORT FOR MONTAGES.
 * NOTES::
 *  Make sure to implement the Server_Landed function to reset values.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType,Blueprintable )
class SYMPLADVANCEDMOVEMENT_API USymplAdvancedMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USymplAdvancedMovementComponent();
	
#pragma region DISPATCHERS

	/**
	 * Notify that the owner has jumped.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FOwnerJump OwnerJump;

	/**
	 * Notify that the auto run state has changed.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FAutoRunStateUpdate AutoRunStateUpdate;

	/**
	 * Notify that we have climbed or wall ran.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidClimb DidClimb;

	/**
	 * Notify that we crouched.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidCrouch DidCrouch;

	/**
	 * Notify that we are sliding.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidSlide DidSlide;

	/**
	 * Notify that we are sprinting.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidSprint DidSprint;

	/**
	 * Notify that we changed our selected speeds.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FSelectedSpeedsUpdate SelectedSpeedsUpdate;

	/**
	 * Notify that we changed our current speed.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FCurrentSpeedUpdate CurrentSpeedUpdate;

	/**
	 * Notify that we changed our movement mode.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FMovementModeUpdate MovementModeUpdate;

	/**
	 * Notify that we switched prone state.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidProne DidProne;

	/**
	 * Notify that we switched hover state.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidHover DidHover;

	/**
	 * Notify that we blinked.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidBlink DidBlink;

	/**
	 * Notify that we rolled.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidRoll DidRoll;

	/**
	 * Notify that we dashed.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidDash DidDash;

	/**
	 * Notify that we toggled our parachute.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidParachute DidParachute;

	/**
	 * Notify that we toggled zero g movement.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidParachute DidZeroG;

	/**
	 * Notify that we toggled our jetpack.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FDidJetpack DidJetpack;

	/**
	 * Notify that we toggled our jetpack.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FJetpackFuelUpdate JetpackFuelUpdate;

	/**
	 * Notify that we have updated animations.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AdvancedMovement")
		FAnimationUpdate AnimationUpdate;

#pragma endregion

#pragma region DEFAULTS



	/**
	 * The actor class that is spawned when the parachute is deployed.
	 * It's really just for visuals by default, though you can run your own custom function if desired.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		TSoftClassPtr<AActor> ParachuteClass;

	/**
	 * The table we use to grab default movement speeds.
	 * If this is null, you will need to set your speeds manually by calling
	 * Server_SetMovementSpeeds.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		UDataTable* SpeedTable;

	/**
	 * This table should have a scalar that adjusts the speed based on an angle.
	 * I.E. if Angle < 45 degrees set speed to .75 or > 45 degress set speed to 1.25.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		UCurveFloat* SlopeSpeedCurve;

	/**
	 * The table row we use to grab animations.
	 * Server_SetMovementAnims.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		FDataTableRowHandle AnimationRow;

	/**
	 * The channel we use to trace for wall collisions.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		TEnumAsByte<ECollisionChannel> WallDetectCollisionChannel;

	/**
	 * The channel we use to trace for slope if we are not a character.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		TEnumAsByte<ECollisionChannel> SlopeTraceChannel;

	/**
	 * The channel we use to trace for a floor.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		TEnumAsByte<ECollisionChannel> FloorTraceChannel;

	/**
	 * Our default movement mode.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		TEnumAsByte<EAdvancedMovementMode> DefaultMovementMode;

	/**
	 * If true, the player will restore jetpack fuel when bJetpackActive == false.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		bool bRestoreJetpackFuelWhenInactive;

	/**
	 * If true, the player can double jump.
	 * This is called after the first jump for n times that the double jump counter allows.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		bool bAllowDoubleJump;

	/**
	 * If true, when double jumping and launching the character, we will override XY velocity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		bool bDoubleJumpXYOverride;

	/**
	 * If true, we will adjust the custom speed for the character every frame.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		bool bManageCustomSpeed;

	/**
	 * If true, we will calculate the angle to the floor that the owner is walking on and scale the speed accordingly.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		bool bAdjustSpeedToSlope;

	/**
	 * If true, we will do the custom slope trace even if the owner is a character.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		bool bForceCustomSlopeTrace;

	/**
	 * If true, this component will do climbing and wall run checks.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		bool bEnableClimbing_WallRun;

	/**
	 * If true, this component will do hovering logic.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		bool bEnableHover;

	/**
	 * If true, when double jumping and launching the character, we will override Z velocity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		bool bDoubleJumpZOverride;

	/**
	 * If true, initialize the component on play.
	 * If false, you will need to initialize it before using it or the game/editor could crash!
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement")
		bool bAutoInit;

	/**
	 * This is a mirror of the CharacterMovementComponent property.
	 * For example: When we are sliding, we set the charactermovement property to false.
	 * When we stop sliding we want to reset the value to default.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Character")
		bool bOrientRotationToMovement;

	/**
	 * If true, the character will try to slide first when doing advanced crouch behavior.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		bool bCanSlide;

	/**
	 * If true, the character will wait for another keypress to uncrouch.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		bool bToggleCrouch;

	/**
	 * If true, the character will wait for another keypress to stop hovering.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		bool bToggleHover;

	/**
	 * If true, the character will wait for another keypress to leave prone.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Prone")
		bool bToggleProne;

	/**
	 * If true, the character will wait for another keypress or for slide time to reach max to release slide.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		bool bToggleSlide;

	/**
	 * If true, the character will wait for another keypress to toggle roll.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Roll")
		bool bToggleRoll;

	/**
	 * If true, the character will wait for another keypress to toggle blink.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Blink")
		bool bToggleBlink;

	/**
	 * If true, the character will wait for another keypress or for the climb time to reach it's max value before resetting the climb/wallrun state.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		bool bToggleWallRun_Climb;

	/**
	 * If true, the player will always call custom jump when doing their first jump.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		bool bIgnoreSlideAngle;

	/**
	 * If true, the player will always call custom jump when doing their first jump.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		bool bForceCustomJump;

	/**
	 * If true, the player will wait for keypress to stop sprinting.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Sprinting")
		bool bToggleSprint;

	/**
	 * If true, the player will wait for keypress to stop dashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Dash")
		bool bToggleDash;

	/**
	 * If true, the player will always call custom crouch when doing their advanced crouch.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		bool bForceCustomCrouch;

	/**
	 * The number of times the player can double jump before landing.
	 * This value starts from 0 so a value of 0 = 1 double jump.
	 * To disable double jumping set bAllowDoubleJump to false.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		int32 AllowedNumberOfDoubleJumps;

	/**
	 * The radius for the sphere sweep that checks for a wall.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double WallDetectTraceRadius;

	/**
	 * The radius for the sphere sweep that checks for a wall.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double RequiredSlideAngle;

	/**
	 * The maximum amount of time a player can be wall running or climbing before the system resets.
	 * Set this to a value <= 0 to make it infinite.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double MaxJetpackFuel;

	/**
	 * The force of the jetpack.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double JetpackForce;

	/**
	 * The amount of fuel that the jetpack needs to activate.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double RequiredFuelForJetpack;

	/**
	 * The rate that the jetpack drains every frame
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double JetpackDrainRate;

	/**
	 * The rate that the jetpack refuels every frame.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double JetpackRefuelRate;

	/**
	 * The maximum amount of time a player can be wall running or climbing before the system resets.
	 * Set this to a value <= 0 to make it infinite.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double MaxWallRun_ClimbTime;

	/**
	 * The maximum amount of time a player can be blinking
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Blink")
		double MaxBlinkTime;

	/**
	 * The value that we set a character's braking friction when sliding.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		double SlideBrakingFriction;

	/**
	 * The maximum amount of time a player can be dashing
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Dash")
		double MaxDashTime;

	/**
	 * The maximum amount of time a player can be hovering
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Hover")
		double MaxHoverTime;

	/**
	 * The maximum amount of time a player can be rolling
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Roll")
		double MaxRollTime;

	/**
	 * The maximum amount of time a player can be sliding before the system resets.
	 * Set this to a value <= 0 to make it infinite.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		double MaxSlideTime;

	/**
	 * The force to add to the slide.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		double SlideForce;

	/**
	 * The force to add to the slide.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Sprinting")
		double SprintSpeed;

	/**
	 * Used to scale the launch velocity when we finish wall run checks.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		double WallRun_ClimbLaunchVelocityScalar;

	/**
	 * The owner's velocity that is needed to be considered valid for sliding.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Crouching/Sliding")
		double RequiredSlideSpeed;

	/**
	 * The owner's max acceleration when parachuting.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Parachute")
		double ParachuteMaxAcceleration;

	/**
	 * The owner's distance to the nearest floor that is needed to deploy the parachute.
	 * This is called separately in CanDeployParachute.
	 * By default CanDelpoyParachute is not implemented in this component.
	 * I have decided to separate it so you can implement it yourself since I see a lot of use cases where you will want to do it in other ways.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Parachute")
		double RequiredDistanceToDeployParachute;

	/**
	 * The distance we trace from our owner location to find the floor.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Movement")
		double FloorCheckDistance;

	/**
	 * The force we add to our blink velocity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Blink")
		double BlinkForce;

	/**
	 * The force we scale to our dash velocity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Dash")
		double DashForce;

	/**
	 * The force we add to our roll velocity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Dash")
		double RollForce;

	/**
	 * The socket we attach the parachute actor to.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Parachute")
		FName ParachuteAttachSocket;

	/**
	 * Component tag for front wall checker.
	 * This looks for a scene component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		FName FrontWallCheckTag;

	/**
	 * Component tag for right wall checker.
	 * This looks for a scene component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		FName RightWallCheckTag;

	/**
	 * Component tag for left wall checker.
	 * This looks for a scene component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Climbing/WallRun")
		FName LeftWallCheckTag;

	/**
	 * The default capsule size for the character.
	 * X = Radius
	 * Y = Height
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Character")
		FVector2D DefaultCapsuleSize;

	/**
	 * The mesh location for doing things like going prone.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Character")
		FVector DefaultMeshLocation;

	/**
	 * The velocity to add to the character when double jumping.
	 * If your owner is a character, this is typically run through the LaunchCharacter function.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		FVector DoubleJumpVelocity;

	/**
	 * The velocity to add to the character when custom jumping.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvancedMovement|Jumping")
		FVector CustomJumpVelocity;

#pragma endregion

protected:

#pragma region CUSTOMOVERRIDES

	/**
	 * BP Overridable function for custom jump (typically used if owner is not a pawn or if bForceCustomJump is implemented).
	 * By default it calls the virtual void function CustomJump.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AdvancedMovement|Internal")
		void CustomJump(bool bPressed, bool bDoubleJump);
	virtual void CustomJump_Impl(bool bPressed, bool bDoubleJump); //C++ override for custom jump.

	/**
	 * BP Overridable function for custom crouch (typically used if owner is not a pawn or if bForceCustomJump is implemented).
	 * By default it calls the virtual void function CustomJump.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AdvancedMovement|Internal")
		void CustomCrouch(bool bPressed, bool bIsSliding);
	virtual void CustomCrouch_Impl(bool bPressed, bool bIsSliding); //C++ override for custom crouch.

	virtual bool FrontClimbCheck(double DeltaTime); //Check front climb.
	virtual bool LeftClimbCheck(double DeltaTime); //Check left climb.
	virtual bool RightClimbCheck(double DeltaTime); //Check right climb.
	virtual bool ClimbCheck(USceneComponent* WallDetect, TEnumAsByte<EMovementAnimType> MovementType, double DeltaTime, FVector Velocity); //Trace to find start location
	//virtual void DoClimb(TEnumAsByte<EMovementAnimType> AnimType, FVector LaunchVelocity, double DeltaTime); //Actually do the climb.

#pragma endregion

#pragma region UEOVERRIDES

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion

#pragma region NETWORKING

	/**
	 * Move owner to transform.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Jetpack")
		void Server_UpdateTransform(FTransform Transform);
	bool Server_UpdateTransform_Validate(FTransform Transform);

	/**
	 * Set current jetpack fuel.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Jetpack")
		void Server_SetJetpackFuel(double Value);
	bool Server_SetJetpackFuel_Validate(double Value);

	/**
	 * Set enable zero g movement.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetZeroGMovement(bool bZeroG);
	bool Server_SetZeroGMovement_Validate(bool bZeroG);

	/**
	 * Set set enable jetpack.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetJetpack(bool bJetpack);
	bool Server_SetJetpack_Validate(bool bJetpack);

	/**
	 * Set input value.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetForwardInput(double Value);
	bool Server_SetForwardInput_Validate(double Value);

	/**
	 * Set input value.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetRightInput(double Value);
	bool Server_SetRightInput_Validate(double Value);

	/**
	 * Set input value.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetUpInput(double Value);
	bool Server_SetUpInput_Validate(double Value);

	/**
	 * Deploy Parachute
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_DeployParachute();
	bool Server_DeployParachute_Validate();

	/**
	 * Deploy Parachute
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_ReleaseParachute();
	bool Server_ReleaseParachute_Validate();

	/**
	 * Set is prone.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetProne(bool bPressed);
	bool Server_SetProne_Validate(bool bPressed);

	/**
	 * Set is Hovering.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetHovering(bool bPressed, bool bForceEndHover);
	bool Server_SetHovering_Validate(bool bPressed, bool bForceEndHover);

	/**
	 * Change the movement mode.
	 * Movement speeds are updated every frame.
	 * This really only updates speed for the owner. 
	 * If you want to adjust movement types you will still want to use character movement set movemement mode or your custom logic.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Movement")
		void Server_SetMovementMode(EAdvancedMovementMode Mode);
	bool Server_SetMovementMode_Validate(EAdvancedMovementMode Mode);

	/**
	 * Update your speeds.
	 * This will also call Server_SetMovementMode if bForceSetMovementMode == true.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Movement")
		void Server_SetMovementSpeeds(const TArray<FSymplMovementSpeeds>& InSpeeds, EAdvancedMovementMode NewMode = EAdvancedMovementMode::EWALK, bool bForceSetMovementMode = true);
	bool Server_SetMovementSpeeds_Validate(const TArray<FSymplMovementSpeeds>& InSpeeds, EAdvancedMovementMode NewMode = EAdvancedMovementMode::EWALK, bool bForceSetMovementMode = true);

	/**
	 * Do the actual climb function.
	 * This launches the character, sets the movement type and sets our climb time from delta time.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_DoClimb(EMovementAnimType AnimType, FVector LaunchVelocity, double DeltaTime);
	bool Server_DoClimb_Validate(EMovementAnimType AnimType, FVector LaunchVelocity, double DeltaTime);

	/**
	 * Do an advanced jump.
	 * This will call character jump, if owned by a character (or a custom jump function if not a character)
	 * If the player has already done a jump then it will try the double jump.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_AdvancedJump(bool bPressed);
	bool Server_AdvancedJump_Validate(bool bPressed);

	/**
	 * Notify that the owner has landed.
	 * Call this when your component owner hits the ground (i.e. Character->OnLanded).
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement")
		void Server_Landed();
	bool Server_Landed_Validate();

	/**
	 * Initialize the component.
	 * Will be called on play if bAutoInit == true
	 * If you set it to false, you will need to initialize the component before using it our the editor/game could crash.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement")
		void Server_Initialize();
	bool Server_Initialize_Validate();

	/**
	 * Set auto run state enabled.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Input")
		void Server_SetAutoRunEnabled(bool bEnabled);
	bool Server_SetAutoRunEnabled_Validate(bool bEnabled);

	/**
	 * Climb checker.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Climbing")
		void Server_FrontCheck_Climb(double DeltaTime);
	bool Server_FrontCheck_Climb_Validate(double DeltaTime);

	/**
	 * Climb checker.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Climbing")
		void Server_LeftCheck_Climb(double DeltaTime);
	bool Server_LeftCheck_Climb_Validate(double DeltaTime);

	/**
	 * Climb checker.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Climbing")
		void Server_RightCheck_Climb(double DeltaTime);
	bool Server_RightCheck_Climb_Validate(double DeltaTime);

	/**
	 * Handle crouch/sliding.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Crouching/Sliding")
		void Server_AdvancedCrouch(bool bPressed);
	bool Server_AdvancedCrouch_Validate(bool bPressed);

	/**
	 * Handle crouch/sliding.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Sprinting")
		void Server_Sprint(bool bPressed);
	bool Server_Sprint_Validate(bool bPressed);

	/**
	 * Handle dashing.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Dash")
		void Server_Dash(bool bPressed, FVector Direction, bool bForceEnd);
	bool Server_Dash_Validate(bool bPressed, FVector Direction, bool bForceEnd);

	/**
	 * Handle blinking.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Blink")
		void Server_Blink(bool bPressed, FVector Direction, bool bForceEnd);
	bool Server_Blink_Validate(bool bPressed, FVector Direction, bool bForceEnd);

	/**
	 * Handle blinking.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Roll")
		void Server_Roll(bool bPressed, FVector Direction, bool bForceEnd);
	bool Server_Roll_Validate(bool bPressed, FVector Direction, bool bForceEnd);

	/**
	 * Handle movement animations.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Movement")
		void Server_SetMovementAnimations(FSymplMovementAnimations Animations);

	/**
	 * Handle movement animations.
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "AdvancedMovement|Animations")
		void Server_PlayMontage(UAnimMontage* Montage, bool bUseAnimInstance = true, double PlayRate = 1.f, double StartPosition = 0.f, bool bStopMontages = true, FName StartSectionName = "");
	bool Server_PlayMontage_Validate(UAnimMontage* Montage, bool bUseAnimInstance = true, double PlayRate = 1.f, double StartPosition = 0.f, bool bStopMontages = true, FName StartSectionName = "");

	/**
	 * Handle movement animations.
	*/
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "AdvancedMovement|Animations")
		void Multicast_PlayMontage(UAnimMontage* Montage, bool bUseAnimInstance = true, double PlayRate = 1.f, double StartPosition = 0.f, bool bStopMontages = true, FName StartSectionName = "");

	/**
	 * Crouch on the client.
	*/
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "AdvancedMovement|Animations")
		void Client_Crouch(bool bPressed);

	/**
	 * Jump on the client.
	*/
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "AdvancedMovement|Animations")
		void Client_Jump(bool bPressed);


#pragma endregion

#pragma region STANDARDFUNCTIONS

	/**
	 * If auto run is enabled, set it to false else set it to true.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Input")
		bool ToggleAutoRunEnabled();

	/**
	 * Set movement mode to last movement mode.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Movement")
		void RestoreLastMovementMode();

	/**
	 * Set character movement mode to last character movement mode.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Movement")
		void RestoreLastCharacterMovementMode();

	/**
	 * Handle movement animations.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Animations")
		double ReplicatedMontage(UAnimMontage* Montage, bool bUseAnimInstance = true, double PlayRate = 1.f, double StartPosition = 0.f, bool bStopMontages = true, FName StartSectionName = "");

	/**
	 * Call replicatedmontage from FSymplMovementAnimation.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Animations")
		double ReplicatedMontage_FromAnimStruct(FSymplMovementAnimation Anim);

	/**
	 * Movement input function.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Input")
		void AdvancedMovementInput_Forward(double Value, bool bForce);

	/**
	 * Movement input function.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Input")
		void AdvancedMovementInput_Right(double Value, bool bForce);
	
	/**
	 * Movement input function.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Input")
		void AdvancedMovementInput_Up(double Value, bool bForce);

	/**
	 * Move the player to the last ground location.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Input")
		void MoveToLastGroundLocation();

	/**
	 * Move the player to the last air location.
	*/
	UFUNCTION(BlueprintCallable, Category = "AdvancedMovement|Input")
		void MoveToLastAirLocation();

#pragma endregion

public:

#pragma region GETTERS

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "OwnerAsChar"))
		ACharacter* GetOwnerAsChar() { return OwnerAsChar; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "OwnerAsPawn"))
		APawn* GetOwnerAsPawn() { return OwnerAsPawn; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "ParachuteActor"))
		AActor* GetParachuteActor() { return ParachuteActor; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "FrontWallDetect"))
		USceneComponent* GetFrontWallDetect() { return FrontWallDetect; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "RightWallDetect"))
		USceneComponent* GetRightWallDetect() { return RightWallDetect; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LeftWallDetect"))
		USceneComponent* GetLeftWallDetect() { return LeftWallDetect; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "MovementMode"))
		TArray<FSymplMovementSpeeds> GetSelectedSpeeds() { return SelectedSpeeds; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "MovementMode"))
		FSymplMovementSpeeds GetCurrentMovementSpeed() { return CurrentMovementSpeed; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentMovementAnimations"))
		FSymplMovementAnimations GetCurrentMovementAnimations() { return CurrentMovementAnimations; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure,Category = "AdvancedMovement|Getters",meta = (CompactNodeTitle = "MovementType"))
		TEnumAsByte<EMovementAnimType> GetCurrentMovementType() { return CurrentMovementType; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "MovementMode"))
		TEnumAsByte<EAdvancedMovementMode> GetCurrentMovementMode() { return CurrentMovementMode; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LastMovementMode"))
		TEnumAsByte<EAdvancedMovementMode> GetLastMovementMode() { return LastMovementMode; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LastCharacterMovementMode"))
		TEnumAsByte<EMovementMode> GetLastCharacterMovementMode() { return LastCharacterMovementMode; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "ZeroG"))
		bool ZeroGMovement() { return bZeroGMovement; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "JetpackActive"))
		bool JetpackActive() { return bJetpackActive; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure,Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "DidJump"))
		bool DidJump() { return bDidJump; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "IsParachuting"))
		bool IsParachuting() { return bIsParachuting; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "IsInitialized"))
		bool IsInitialized() { return bInitialized; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "AutoRunEnabled"))
		bool IsAutoRunEnabled() { return bAutoRunEnabled; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Sliding"))
		bool IsSliding() { return bSliding; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Crouching"))
		bool IsCrouching() { return bCrouching; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Dashing"))
		bool IsDashing() { return bDashing; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Blinking"))
		bool IsBlinking() { return bBlinking; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Hovering"))
		bool IsHovering() { return bHovering; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Sprinting"))
		bool IsSprinting() { return bSprinting; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Prone"))
		bool IsProne() { return bProne; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "DoubleJumpCounter"))
		int32 GetDoubleJumpCounter() { return DoubleJumpCounter; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Forward"))
		double GetForwardInput() { return ForwardInput; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Right"))
		double GetRightInput() { return RightInput; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "Up"))
		double GetUpInput() { return UpInput; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentJetpackFuel"))
		double GetCurrentJetpackFuel() { return CurrentJetpackFuel; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "ClimbTime"))
		double GetClimbTime() { return ClimbTime; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "SlideTime"))
		double GetSlideTime() { return SlideTime; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "HoverTime"))
		double GetHoverTime() { return HoverTime; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LastBrakingFriction"))
		double GetLastBrakingFriction() { return LastBrakingFriction; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "SlopeAngle"))
		double GetCurrentSlopeAngle () { return CurrentSlopeAngle; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentSlopeSpeedScalar"))
		double GetCurrentSlopeSpeedScalar() { return CurrentSlopeSpeedScalar; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LastMaxAcceleration"))
		double GetLastMaxAcceleration() {return LastMaxAcceleration;}

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentDashDirection"))
		FVector GetCurrentDashDirection() {return CurrentDashDirection;}

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentBlinkDirection"))
		FVector GetCurrentBlinkDirection() { return CurrentBlinkDirection; }
	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentRollDirection"))
		FVector GetCurrentRollDirection() { return CurrentRollDirection; }
	
	/**
	 * Return FVector(Forward,Right,Up).
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "InputDir"))
		FVector GetInputDirection() { return FVector(ForwardInput,RightInput,UpInput); }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LastGroundLocation"))
		FVector GetLastGroundLocation() { return LastGroundLocation; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "LastAirLocation"))
		FVector GetLastAirLocation() { return LastAirLocation; }

	/**
	 * Return the value.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Getters", meta = (CompactNodeTitle = "CurrentVelocity"))
		FVector GetCurrentVelocity() { return CurrentVelocity; }


#pragma endregion

#pragma region CONDITIONS

	/**
	 * A standard input blocker if the character is sliding, dashing, rolling or hovering.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Jumping", meta = (CompactNodeTitle = "CanDoubleJump"))
		bool CanMove() { return !bSliding && !bDashing && !bRolling && !bHovering; }

	/**
	 * True if the player can deploy their parachute.
	 * This is called in the implementation to prevent blocking.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Jumping", meta = (CompactNodeTitle = "CanDoubleJump"))
		bool CanDeployParachute();

	/**
	 * True if the player can jump
	*/
	UFUNCTION(BlueprintPure,Category = "AdvancedMovement|Jumping", meta = (CompactNodeTitle = "CanDoubleJump"))
		bool CanDoubleJump() { return bDidJump && GetDoubleJumpCounter() <= AllowedNumberOfDoubleJumps; }

	/**
	 * Check to see if the player can slide by determining velocity.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Sliding", meta = (CompactNodeTitle = "CanSlide"))
		bool CanSlide();

	/**
	 * Check to see if the player can dash by determining velocity.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Sliding", meta = (CompactNodeTitle = "CanDash"))
		bool CanDash() { return !bRolling && !bBlinking && DashTime <= MaxDashTime; }

	/**
	 * Check to see if the player can roll by determining velocity.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Sliding", meta = (CompactNodeTitle = "CanRoll"))
		bool CanRoll() { return !bDashing && !bBlinking && RollTime <= MaxRollTime; }

	/**
	 * Check to see if the player can blink by determining velocity.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Sliding", meta = (CompactNodeTitle = "CanBlink"))
		bool CanBlink() { return !bDashing && !bRolling && BlinkTime <= MaxBlinkTime; }

	/**
	 * Check to see if the player can blink by determining velocity.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement|Sliding", meta = (CompactNodeTitle = "CanBlink"))
		bool CanHover() { return bEnableHover && HoverTime < MaxHoverTime; }

#pragma endregion

	/**
	 * Trace for the floor.
	*/
	UFUNCTION(BlueprintPure, Category = "AdvancedMovement", meta = (CompactNodeTitle = "FindFloor"))
		bool FindFloor(FHitResult& OutHit);

private:

#pragma region PROPERTIES

	//Valid if owner is a character.
	UPROPERTY(Replicated)
		ACharacter* OwnerAsChar;

	//Valid if owner is a pawn.
	UPROPERTY(Replicated)
		APawn* OwnerAsPawn;

	//The fast owner ref.
	UPROPERTY(Replicated)
		AActor* OwnerRef;

	//The current parachute actor.
	UPROPERTY(Replicated)
		AActor* ParachuteActor;

	//Valid if owner is a character.
	UPROPERTY(Replicated)
		USceneComponent* FrontWallDetect;

	//Valid if owner is a pawn.
	UPROPERTY(Replicated)
		USceneComponent* RightWallDetect;

	//The fast owner ref.
	UPROPERTY(Replicated)
		USceneComponent* LeftWallDetect;

	//The movement mode for adjusting speed.
	UPROPERTY(Replicated)
		TArray<FSymplMovementSpeeds> SelectedSpeeds;

	//The movement mode for adjusting speed.
	UPROPERTY(Replicated)
		FSymplMovementSpeeds CurrentMovementSpeed;

	//The movement animations.
	UPROPERTY()
		FSymplMovementAnimations CurrentMovementAnimations;

	//The movement anim type for animations.
	UPROPERTY(Replicated)
		TEnumAsByte<EMovementAnimType> CurrentMovementType;

	//The movement mode for adjusting speed.
	UPROPERTY(Replicated)
		TEnumAsByte<EAdvancedMovementMode> CurrentMovementMode;

	//The last movement mode for adjusting speed.
	UPROPERTY(Replicated)
		TEnumAsByte<EAdvancedMovementMode> LastMovementMode;

	//The last movement mode for the character.
	UPROPERTY(Replicated)
		TEnumAsByte<EMovementMode> LastCharacterMovementMode;

	//Determines if the player is in zero G.
	UPROPERTY(Replicated)
		bool bZeroGMovement;

	//Determines if the player is in zero G.
	UPROPERTY(Replicated)
		bool bJetpackActive;

	//Determines if the player is hovering.
	UPROPERTY(Replicated)
		bool bHovering;

	//Determines if the player is parachuting.
	UPROPERTY(Replicated)
		bool bIsParachuting;

	//Determines if the player did their first jump.
	UPROPERTY(Replicated)
		bool bDidJump;

	//Determines if the character is dashing.
	UPROPERTY(Replicated)
		bool bDashing;

	//Determines if the character is rolling.
	UPROPERTY(Replicated)
		bool bRolling;

	//Determines if the character is blinking.
	UPROPERTY(Replicated)
		bool bBlinking;

	//Determines if the character is prone.
	UPROPERTY(Replicated)
		bool bProne;

	//Determines if the player is autorunning.
	UPROPERTY(Replicated)
		bool bAutoRunEnabled;

	//Determines if the player has initialized the component.
	UPROPERTY(Replicated)
		bool bInitialized;

	//Determines if the player has initialized the component.
	UPROPERTY(Replicated)
		bool bSprinting;

	//Determines if the player is sliding.
	UPROPERTY(Replicated)
		bool bSliding;

	//Determines if the player is sliding.
	UPROPERTY(Replicated)
		bool bCrouching;

	//The number of times the player has double jumped.
	UPROPERTY(Replicated)
		int32 DoubleJumpCounter;

	//Forward movement input.
	UPROPERTY(Replicated)
		double ForwardInput;

	//Right movement input.
	UPROPERTY(Replicated)
		double RightInput;

	//Up movement input.
	UPROPERTY(Replicated)
		double UpInput;

	//The amount of jetpack fuel we have.
	UPROPERTY(Replicated)
		double CurrentJetpackFuel;

	//The angle of our walking slope.
	UPROPERTY(Replicated)
		double CurrentSlopeAngle;

	//The speed scalar for slope angle.
	UPROPERTY(Replicated)
		double CurrentSlopeSpeedScalar;

	//The total time that we have been climbing.
	UPROPERTY(Replicated)
		double ClimbTime;

	//The total time that we have been rolling.
	UPROPERTY(Replicated)
		double RollTime;

	//The total time that we have been dashing.
	UPROPERTY(Replicated)
		double DashTime;

	//The total time that we have been blinking.
	UPROPERTY(Replicated)
		double BlinkTime;

	//The total time that we have been climbing.
	UPROPERTY(Replicated)
		double SlideTime;

	//The total time that we have been climbing.
	UPROPERTY(Replicated)
		double HoverTime;

	//The current movement speed.
	UPROPERTY(Replicated)
		double CurrentSpeed;

	//The last braking friction for a character.
	UPROPERTY(Replicated)
		double LastBrakingFriction;

	//The last max acceleration for a character.
	UPROPERTY(Replicated)
		double LastMaxAcceleration;

	//The current dash direction.
	UPROPERTY(Replicated)
		FVector CurrentDashDirection;

	//The current blink direction.
	UPROPERTY(Replicated)
		FVector CurrentBlinkDirection;

	//The current roll direction.
	UPROPERTY(Replicated)
		FVector CurrentRollDirection;

	//The last location the player was on the ground.
	UPROPERTY(Replicated)
		FVector LastGroundLocation;

	//The last location the player was on the ground.
	UPROPERTY(Replicated)
		FVector LastAirLocation;

	//The current velocity of our player.
	UPROPERTY(Replicated)
		FVector CurrentVelocity;

#pragma endregion

};
