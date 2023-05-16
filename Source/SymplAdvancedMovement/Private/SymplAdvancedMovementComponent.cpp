// Fill out your copyright notice in the Description page of Project Settings.


#include "SymplAdvancedMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "SymplAdvancedMovementInterface.h"

// Sets default values for this component's properties
USymplAdvancedMovementComponent::USymplAdvancedMovementComponent()
{
	// Set defaults.
	PrimaryComponentTick.bCanEverTick = true;
	OwnerAsChar = nullptr;
	OwnerAsPawn = nullptr;
	OwnerRef = nullptr;
	FrontWallDetect = nullptr;
	RightWallDetect = nullptr;
	LeftWallDetect = nullptr;
	SpeedTable = nullptr;
	AnimationRow = FDataTableRowHandle();
	WallDetectCollisionChannel = ECollisionChannel::ECC_Visibility;
	SlopeTraceChannel = ECollisionChannel::ECC_Visibility;
	FloorTraceChannel = ECollisionChannel::ECC_Visibility;
	SelectedSpeeds.Empty();
	CurrentMovementAnimations = FSymplMovementAnimations();
	CurrentMovementSpeed = FSymplMovementSpeeds();
	CurrentMovementType = EMovementAnimType::ENONE;
	DefaultMovementMode = EAdvancedMovementMode::EWALK;
	LastCharacterMovementMode = EMovementMode::MOVE_Walking;
	bInitialized = false;
	bDidJump = false;
	bAllowDoubleJump = true;
	bManageCustomSpeed = true;
	bAutoInit = true;
	bForceCustomJump = false; 
	bDoubleJumpXYOverride = true ;
	bDoubleJumpZOverride = true;
	bEnableClimbing_WallRun = true;
	bCanSlide = true;
	bIgnoreSlideAngle = true;
	bOrientRotationToMovement = false; 
	bIsParachuting = false;
	bToggleCrouch = false;
	bToggleSlide = false;
	bEnableHover = true;
	bToggleWallRun_Climb = false;
	bCrouching = false;
	bSprinting = false;
	bToggleSprint = false;
	bForceCustomSlopeTrace = false;
	bProne = false;
	bToggleProne = true;
	bToggleDash = true;
	bToggleBlink= true;
	bToggleRoll = true;
	bToggleHover = false;
	bAdjustSpeedToSlope = true;
	bRestoreJetpackFuelWhenInactive = true;
	DoubleJumpCounter = 0;
	AllowedNumberOfDoubleJumps = 0;
	ClimbTime = 0.f;
	RollTime = 0.f;
	DashTime = 0.f;
	HoverTime = 0.f;
	RequiredDistanceToDeployParachute = 10000.f;
	WallDetectTraceRadius = 15.f;
	MaxWallRun_ClimbTime = 10.f;
	MaxSlideTime = 10.f;
	RequiredSlideSpeed = 800.f;
	RequiredSlideAngle = 30.f;
	SlideForce = 600.f;
	SprintSpeed = 800.f;
	WallRun_ClimbLaunchVelocityScalar = 600.f;
	CurrentSpeed = 0.f; 
	MaxBlinkTime = 3.f;
	MaxDashTime = 3.f;
	MaxRollTime = 1.f;
	MaxHoverTime = 10.f;
	SlideBrakingFriction = 0.f;
	CurrentSlopeSpeedScalar = 1.f;
	CurrentSlopeAngle = 0.f; 
	ParachuteMaxAcceleration = 2500.f;
	FloorCheckDistance = 100000.f;
	LastMaxAcceleration = 0.f;
	MaxJetpackFuel = 100.f;
	JetpackForce = 10000.f;
	CurrentJetpackFuel = MaxJetpackFuel;
	JetpackDrainRate = 1.f;
	JetpackRefuelRate = .5f;
	RequiredFuelForJetpack = .1f;
	DashForce = 1500.f;
	BlinkForce = 1500.f;
	RollForce = 500.f;
	ParachuteAttachSocket = "ParachuteSocket";
	FrontWallCheckTag = "FrontWallCheck";
	RightWallCheckTag = "RightWallCheck";
	LeftWallCheckTag = "LeftWallCheck"; 
	DefaultCapsuleSize = FVector2D(35.f, 96.f);
	DefaultMeshLocation = FVector(0.f, 0.f, -98.f);
	DoubleJumpVelocity = FVector(0.f,0.f,500.f);
	CustomJumpVelocity = FVector(0.f, 0.f, 500.f);
	LastGroundLocation = FVector();
	LastAirLocation = FVector();
}

// Called when the game starts
void USymplAdvancedMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoInit)
	{
		//Handle auto init.
		Server_Initialize();
	}
}

// Called every frame
void USymplAdvancedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsInitialized())
	{
		if (OwnerAsPawn)
		{
			CurrentVelocity = OwnerAsPawn->GetMovementComponent()->Velocity;
		}
		else
		{
			OwnerRef->GetVelocity();
		}
#pragma region SLOPECALCULATIONS
		if (bAdjustSpeedToSlope && SlopeSpeedCurve)
		{
			if (OwnerAsChar && !bForceCustomSlopeTrace)
			{
				//Get the slope angle from the find floor result.
				CurrentSlopeAngle = FMath::RadiansToDegrees(FMath::Acos(OwnerAsChar->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal.Z));
			}
			else
			{
				//Trace for slope
				FVector up = OwnerRef->GetActorUpVector();
				FVector down = up * -1.0f;

				FHitResult hit;
				FCollisionQueryParams params;
				FCollisionResponseParams response;
				FCollisionShape shape;
				shape.MakeSphere(1.f);
				params.bFindInitialOverlaps = true;

				if (GetWorld()->SweepSingleByChannel(hit, OwnerRef->GetActorLocation(), OwnerRef->GetActorLocation() + down * 100.0f, FQuat(), SlopeTraceChannel, shape, params, response))
				{
					CurrentSlopeAngle = FMath::RadiansToDegrees(FMath::Acos(hit.ImpactNormal | up));
				}
				else
				{
					//No hit so we assume we are on an even plane.
					CurrentSlopeAngle = 0.f;
				}
			}
			//Gather the value from the Curvedouble.
			CurrentSlopeSpeedScalar = SlopeSpeedCurve->GetFloatValue(CurrentSlopeAngle);
		}

#pragma endregion

#pragma region SPEED
		if (bManageCustomSpeed)
		{
			if (SelectedSpeeds.Num() > 0 && CurrentMovementMode != LastMovementMode)
			{
				for (auto& speed : SelectedSpeeds)
				{
					if (speed.MovementMode == CurrentMovementMode)
					{
						CurrentSpeed = speed.Speed * CurrentSlopeSpeedScalar;
						if (OwnerAsChar)
						{
							if (CurrentMovementMode == EAdvancedMovementMode::EFLY)
							{
								OwnerAsChar->GetCharacterMovement()->MaxFlySpeed = CurrentSpeed;
							}
							else if (CurrentMovementMode == EAdvancedMovementMode::ECROUCH)
							{

								OwnerAsChar->GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentSpeed;
							}
							else
							{
								OwnerAsChar->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
							}
						}
					}
					break;
				}
			}
		}
#pragma endregion

#pragma region AUTORUN
		//Handle auto run
		if (IsAutoRunEnabled())
		{
			if (OwnerAsPawn && OwnerRef->GetClass()->ImplementsInterface(USymplAdvancedMovementInterface::StaticClass()))
			{
				//Add pawn movement input to our forward vector if auto run is enabled.
				OwnerAsPawn->AddMovementInput(ISymplAdvancedMovementInterface::Execute_GetOwnerMovementDirection(OwnerRef), 1.f, false);
			}
			else
			{
				if (OwnerRef && OwnerRef->GetClass()->ImplementsInterface(USymplAdvancedMovementInterface::StaticClass()))
				{
					//Add local offset for actors that aren't pawns or characters..
					OwnerRef->AddActorLocalOffset(OwnerRef->GetActorForwardVector() * ISymplAdvancedMovementInterface::Execute_GetOwnerSpeed(OwnerRef));
				}
			}
		}
#pragma endregion

#pragma region CLIMBING
		if (bEnableClimbing_WallRun && bDidJump)
		{
			bool climb = false;
			if (OwnerAsPawn)
			{
				climb = OwnerAsPawn->GetMovementComponent()->IsFalling();
			}
			if (climb)
			{
				//Disable climbing animation type.
				if (!FrontClimbCheck(DeltaTime) && !RightClimbCheck(DeltaTime) && !LeftClimbCheck(DeltaTime))
				{
					CurrentMovementType = EMovementAnimType::ENONE;
				}
			}
		}
#pragma endregion

#pragma region SLIDING
		//Slide on server only.
		if (GetOwner()->GetLocalRole() >= ROLE_Authority)
		{
			if (bCanSlide)
			{
				if (CanSlide() && bSliding)
				{
					//Increment slide time.
					SlideTime += DeltaTime;
					//Set movement anim type.
					CurrentMovementType = EMovementAnimType::ESLIDING;
					if (OwnerAsChar)
					{
						//Set movement defaults for character.
						OwnerAsChar->GetCharacterMovement()->bOrientRotationToMovement = false;
						OwnerAsChar->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
					}
					//Set slide direction and add force.
					FVector dir = OwnerRef->GetActorForwardVector();
					dir.Normalize();
					if (OwnerAsChar)
					{
						OwnerAsChar->GetCharacterMovement()->BrakingFriction = SlideBrakingFriction;
						OwnerAsChar->GetCharacterMovement()->AddForce(dir * SlideForce);
					}
					else
					{
						OwnerRef->AddActorLocalOffset(dir * SlideForce);
					}
				}
			}
			else
			{
				//Reset sliding.
				CurrentMovementType = EMovementAnimType::ENONE;
				if (OwnerAsChar)
				{
					OwnerAsChar->GetCharacterMovement()->BrakingFriction = LastBrakingFriction;
					OwnerAsChar->GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
					OwnerAsChar->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				}
				SlideTime = 0.f;
			}
		}
#pragma endregion

#pragma region DASHING
		//Handle dashing.
		if (bDashing && CanDash())
		{
			if (OwnerAsChar)
			{
				OwnerAsChar->LaunchCharacter((CurrentDashDirection * DashForce),true,true);
			}
			else
			{
				OwnerRef->AddActorLocalOffset(CurrentDashDirection * CurrentSpeed);
			}
			DashTime += DeltaTime;
		}
		else
		{
			DashTime = 0.f;
			if(bDashing)
				Server_Dash(false, FVector(), true);
		}
#pragma endregion

#pragma region BLINKING
		//Handle blinking.
		if (bBlinking && CanBlink())
		{
			if (OwnerAsChar)
			{
				OwnerAsChar->LaunchCharacter((CurrentBlinkDirection * BlinkForce),true,true);
			}
			else
			{
				OwnerRef->AddActorLocalOffset(CurrentDashDirection * CurrentSpeed);
			}
			BlinkTime += DeltaTime;
		}
		else
		{
			BlinkTime = 0.f;
			if(bBlinking)
				Server_Blink(false, FVector(), true);
		}
#pragma endregion

#pragma region ROLLING
		//Handle rolling.
		if (bRolling && CanRoll())
		{
			if (OwnerAsChar)
			{
				OwnerAsChar->AddMovementInput((CurrentRollDirection * RollForce),1.f,false);
			}
			else
			{
				OwnerRef->AddActorLocalOffset(CurrentRollDirection * CurrentSpeed);
			}
			RollTime += DeltaTime;
		}
		else
		{
			RollTime = 0.f;
			if(bRolling)
				Server_Roll(false, FVector(), true);
		}
#pragma endregion

#pragma region HOVERING
		if (bHovering && CanHover())
		{
			HoverTime += DeltaTime;
		}
		else
		{
			if(bHovering)
				Server_SetHovering(false, true);
		}
#pragma endregion

#pragma region ZEROG

		if (bZeroGMovement)
		{
			// Normalize the input vector
			FVector InputVector = FVector(ForwardInput, RightInput, UpInput).GetClampedToMaxSize(1.0f);

			// Get the character's rotation and apply it to the input vector
			FVector CharacterDirection = OwnerRef->GetActorRotation().RotateVector(InputVector);

			// Apply the movement to the character
			FVector NewLocation = OwnerRef->GetActorLocation() + CharacterDirection * CurrentSpeed * DeltaTime;
			OwnerRef->SetActorLocation(NewLocation);
		}

	}
#pragma endregion

#pragma region JETPACK

	if (bJetpackActive && CurrentJetpackFuel >= RequiredFuelForJetpack)
	{
		FVector vel = FVector::UpVector * JetpackForce;
		vel += CurrentVelocity;
		if (OwnerAsChar)
		{
			OwnerAsChar->GetCharacterMovement()->Velocity = vel;
		}
		else
		{
			if (OwnerRef->GetRootComponent()->IsSimulatingPhysics())
			{
				Cast<UPrimitiveComponent>(OwnerRef->GetRootComponent())->AddForce(vel);
			}
			else
			{
				OwnerRef->AddActorLocalOffset(vel);
			}
		}
		if(CurrentJetpackFuel != 0.f)
			Server_SetJetpackFuel(CurrentJetpackFuel-JetpackDrainRate);
	}
	else
	{
		if (bRestoreJetpackFuelWhenInactive && CurrentJetpackFuel != MaxJetpackFuel)
			Server_SetJetpackFuel(CurrentJetpackFuel + JetpackRefuelRate);
	}

#pragma endregion

#pragma region LOCATIONS

	if(OwnerRef->GetClass()->ImplementsInterface(USymplAdvancedMovementInterface::StaticClass()))
	{
		bool falling = ISymplAdvancedMovementInterface::Execute_SymplIsFalling(OwnerRef);
		if (!falling)
		{
			LastGroundLocation = OwnerRef->GetActorLocation();
		}
		else
		{
			LastAirLocation = OwnerRef->GetActorLocation();
		}
	}

#pragma endregion

}

// Replication
void USymplAdvancedMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//Set replication.
	DOREPLIFETIME(USymplAdvancedMovementComponent, OwnerAsChar);
	DOREPLIFETIME(USymplAdvancedMovementComponent, OwnerAsPawn);
	DOREPLIFETIME(USymplAdvancedMovementComponent, OwnerRef);
	DOREPLIFETIME(USymplAdvancedMovementComponent, ParachuteActor);
	DOREPLIFETIME(USymplAdvancedMovementComponent, FrontWallDetect);
	DOREPLIFETIME(USymplAdvancedMovementComponent, RightWallDetect);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LeftWallDetect);
	DOREPLIFETIME(USymplAdvancedMovementComponent, SelectedSpeeds);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentMovementSpeed);
	//DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentMovementAnimations);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentMovementType);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentMovementMode);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LastMovementMode);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LastCharacterMovementMode);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bZeroGMovement);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bJetpackActive);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bAutoRunEnabled);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bInitialized);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bIsParachuting);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bHovering);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bDidJump);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bDashing);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bRolling);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bBlinking);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bProne);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bSprinting);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bSliding);
	DOREPLIFETIME(USymplAdvancedMovementComponent, bCrouching);
	DOREPLIFETIME(USymplAdvancedMovementComponent, DoubleJumpCounter);
	DOREPLIFETIME(USymplAdvancedMovementComponent, ForwardInput);
	DOREPLIFETIME(USymplAdvancedMovementComponent, RightInput);
	DOREPLIFETIME(USymplAdvancedMovementComponent, UpInput);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentJetpackFuel);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentSlopeAngle);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentSlopeSpeedScalar);
	DOREPLIFETIME(USymplAdvancedMovementComponent, ClimbTime);
	DOREPLIFETIME(USymplAdvancedMovementComponent, RollTime);
	DOREPLIFETIME(USymplAdvancedMovementComponent, DashTime);
	DOREPLIFETIME(USymplAdvancedMovementComponent, BlinkTime);
	DOREPLIFETIME(USymplAdvancedMovementComponent, SlideTime);
	DOREPLIFETIME(USymplAdvancedMovementComponent, HoverTime);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentSpeed);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LastMaxAcceleration);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LastBrakingFriction);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentDashDirection);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentBlinkDirection);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentRollDirection);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LastGroundLocation);
	DOREPLIFETIME(USymplAdvancedMovementComponent, LastAirLocation);
	DOREPLIFETIME(USymplAdvancedMovementComponent, CurrentVelocity);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void USymplAdvancedMovementComponent::CustomJump_Implementation(bool bPressed, bool bDoubleJump)
{
	//Call c++ function by default.
	CustomJump_Impl(bPressed, bDoubleJump);
}

void USymplAdvancedMovementComponent::CustomJump_Impl(bool bPressed, bool bDoubleJump)
{
	//Basic custom jump.
	if (!OwnerRef)
	{
		return;
	}
	Server_SetMovementMode(EAdvancedMovementMode::EJUMP);
	OwnerRef->AddActorLocalOffset(CustomJumpVelocity);
	OwnerJump.Broadcast(this, true, bDoubleJump);
}

bool USymplAdvancedMovementComponent::ClimbCheck(USceneComponent* WallDetect, TEnumAsByte<EMovementAnimType> MovementType, double DeltaTime, FVector Velocity)
{
	//Sweep for walls and run wall run or climb.
	bool success = false;
	FHitResult hit;
	FVector loc = FVector();
	FCollisionShape shape;
	FCollisionQueryParams params;
	FCollisionResponseParams response;
	shape.MakeSphere(WallDetectTraceRadius);
	params.AddIgnoredActor(OwnerRef);
	if (WallDetect && bDidJump)
	{
		loc = WallDetect->GetComponentLocation();
		success = GetWorld()->SweepSingleByChannel(hit, OwnerRef->GetActorLocation(), loc, FQuat(), WallDetectCollisionChannel, shape, params, response);
		if (success)
		{
			UE_LOG(LogTemp,Warning,TEXT("%s"), *hit.ToString());
			if (MaxWallRun_ClimbTime <= 0 || ClimbTime < MaxWallRun_ClimbTime)
			{
				//Do climb.
				Server_DoClimb(MovementType, Velocity, DeltaTime);
			}
		}
		return success;
	}
	return false;
}

void USymplAdvancedMovementComponent::Server_DoClimb_Implementation(EMovementAnimType AnimType, FVector LaunchVelocity, double DeltaTime)
{
	//Climb movement.
 	if (OwnerAsChar)
	{
		OwnerAsChar->LaunchCharacter(LaunchVelocity, true, true);
	}
	else
	{
		CustomJump(true, true);
	}
	Server_SetMovementMode(EAdvancedMovementMode::ECLIMBING);
	CurrentMovementType = AnimType;
	ClimbTime += DeltaTime;
	DidClimb.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_DoClimb_Validate(EMovementAnimType AnimType, FVector LaunchVelocity, double DeltaTime){ return true;}

void USymplAdvancedMovementComponent::Server_AdvancedJump_Implementation(bool bPressed)
{
	//Jump or climb.
	if (OwnerAsChar && !bForceCustomJump)
	{
		if (!bPressed)
		{
			RestoreLastMovementMode();
			OwnerAsChar->StopJumping();
			Client_Jump(false);
			return;
		}
		if (CanDoubleJump())
		{
			Server_SetMovementMode(EAdvancedMovementMode::EJUMP);
			DoubleJumpCounter++;
			OwnerAsChar->LaunchCharacter(DoubleJumpVelocity + CurrentVelocity,bDoubleJumpXYOverride,bDoubleJumpZOverride);
			ReplicatedMontage_FromAnimStruct(CurrentMovementAnimations.DoubleJumpAnim);
			OwnerJump.Broadcast(this, false, true);
		}
		else
		{
			Server_SetMovementMode(EAdvancedMovementMode::EJUMP);
			OwnerAsChar->Jump();
			Client_Jump(true);
			bDidJump = true;
			OwnerJump.Broadcast(this, false, false);
		}
		return;
	}
	CustomJump(bPressed, CanDoubleJump());
}

bool USymplAdvancedMovementComponent::Server_AdvancedJump_Validate(bool bPressed) { return true; }

void USymplAdvancedMovementComponent::Server_Landed_Implementation()
{
	//Reset values.
	bDidJump = false;
	DoubleJumpCounter = 0;
	ClimbTime = 0.f;
	RestoreLastMovementMode();
	Server_Blink(false, FVector(), true);
	Server_SetHovering(false, true);
}

bool USymplAdvancedMovementComponent::Server_Landed_Validate() { return true; }

void USymplAdvancedMovementComponent::Server_Initialize_Implementation()
{
	//Set values.
	OwnerAsChar = Cast<ACharacter>(GetOwner());
	OwnerAsPawn = Cast<APawn>(GetOwner());
	OwnerRef = GetOwner();
	//Gather wall checkers.
	TArray<UActorComponent*> found;
	found = OwnerRef->GetComponentsByTag(USceneComponent::StaticClass(), FrontWallCheckTag);
	if (found.Num() <= 0)
	{
		return;
	}
	FrontWallDetect = Cast<USceneComponent>(found[0]);
	found = OwnerRef->GetComponentsByTag(USceneComponent::StaticClass(), LeftWallCheckTag);
	if (found.Num() <= 0)
	{
		return;
	}
	LeftWallDetect = Cast<USceneComponent>(found[0]);
	found = OwnerRef->GetComponentsByTag(USceneComponent::StaticClass(), RightWallCheckTag);
	if (found.Num() <= 0)
	{
		return;
	}
	RightWallDetect = Cast<USceneComponent>(found[0]);
	//Set speeds
	if (SpeedTable)
	{
		TArray<FSymplMovementSpeeds> speeds;
		for (auto& name : SpeedTable->GetRowNames())
		{
			speeds.Add(*SpeedTable->FindRow<FSymplMovementSpeeds>(name, ""));
		}
		Server_SetMovementSpeeds(speeds, DefaultMovementMode, true);
	}
	if (AnimationRow.DataTable)
	{
		Server_SetMovementAnimations(*AnimationRow.DataTable->FindRow<FSymplMovementAnimations>(AnimationRow.RowName, ""));
	}
	bInitialized = true;
}

bool USymplAdvancedMovementComponent::Server_Initialize_Validate() { return true; }

void USymplAdvancedMovementComponent::Server_SetAutoRunEnabled_Implementation(bool bEnabled)
{
	//Set auto run.
	Server_SetMovementMode(bEnabled ? EAdvancedMovementMode::ESPRINT : LastMovementMode);
	bAutoRunEnabled = bEnabled;
	AutoRunStateUpdate.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetAutoRunEnabled_Validate(bool bEnabled) { return true; }

bool USymplAdvancedMovementComponent::ToggleAutoRunEnabled()
{
	//Toggle auto run.
	bAutoRunEnabled = !bAutoRunEnabled;
	return IsAutoRunEnabled();
}

void USymplAdvancedMovementComponent::Server_FrontCheck_Climb_Implementation(double DeltaTime)
{
	//Front climb check.
	ClimbCheck(FrontWallDetect, EMovementAnimType::ECLIMBFRONT, DeltaTime, FVector());
}

bool USymplAdvancedMovementComponent::Server_FrontCheck_Climb_Validate(double DeltaTime) { return true;}

void USymplAdvancedMovementComponent::Server_LeftCheck_Climb_Implementation(double DeltaTime)
{
	//Left climb check.
	ClimbCheck(LeftWallDetect, EMovementAnimType::ECLIMBLEFT, DeltaTime, FVector());
}

bool USymplAdvancedMovementComponent::Server_LeftCheck_Climb_Validate(double DeltaTime) { return true;}

void USymplAdvancedMovementComponent::Server_RightCheck_Climb_Implementation(double DeltaTime)
{
	//Right climb check.
	ClimbCheck(RightWallDetect, EMovementAnimType::ECLIMBRIGHT, DeltaTime, FVector());
}

bool USymplAdvancedMovementComponent::Server_RightCheck_Climb_Validate(double DeltaTime) { return true;}

bool USymplAdvancedMovementComponent::FrontClimbCheck(double DeltaTime)
{
	//Front climb check on client.
	Server_FrontCheck_Climb(DeltaTime);
	return ClimbCheck(FrontWallDetect, EMovementAnimType::ECLIMBFRONT, DeltaTime, FVector(0.f,0.f,WallRun_ClimbLaunchVelocityScalar));
}

bool USymplAdvancedMovementComponent::LeftClimbCheck(double DeltaTime)
{
	//Left climb check on client.
	Server_LeftCheck_Climb(DeltaTime);
	return ClimbCheck(LeftWallDetect, EMovementAnimType::ECLIMBLEFT, DeltaTime, LeftWallDetect->GetForwardVector() * WallRun_ClimbLaunchVelocityScalar);
}

bool USymplAdvancedMovementComponent::RightClimbCheck(double DeltaTime)
{
	//Right climb check on client.
	Server_RightCheck_Climb(DeltaTime);
	return ClimbCheck(RightWallDetect, EMovementAnimType::ECLIMBRIGHT, DeltaTime, RightWallDetect->GetForwardVector()*WallRun_ClimbLaunchVelocityScalar);
}

void USymplAdvancedMovementComponent::Server_AdvancedCrouch_Implementation(bool bPressed)
{
	//Crouch or slide.
	if (bForceCustomCrouch)
	{
		CustomCrouch(bPressed,CanSlide());
		return;
	}
	if (bPressed)
	{
		if(CanSlide())
		{
			if (OwnerAsChar)
			{
				bSliding = !bSliding;
				if (bSliding && OwnerAsChar)
				{
					LastBrakingFriction = OwnerAsChar->GetCharacterMovement()->BrakingFriction;
				}
				Server_SetMovementMode(bSliding ? EAdvancedMovementMode::EJUMP : LastMovementMode);
			}
			else
			{
				Server_SetMovementMode(EAdvancedMovementMode::ECROUCH);
				CustomCrouch(bPressed, true);
			}
		}
		else if (OwnerAsPawn->GetMovementComponent()->IsFalling())
		{
			Server_Blink(bPressed, OwnerRef->GetActorForwardVector(), false);
		}
		else
		{
			if (OwnerAsChar)
			{
				bCrouching = !bCrouching;
				Server_SetMovementMode(bCrouching ? EAdvancedMovementMode::ECROUCH : LastMovementMode);
				if (bCrouching)
				{
					OwnerAsChar->Crouch();
					Client_Crouch(true);
				}
				else
				{
					OwnerAsChar->UnCrouch();
					Client_Crouch(false);
				}
			}
			else
			{
				Server_SetMovementMode(EAdvancedMovementMode::ECROUCH);
				CustomCrouch(bPressed, false);
			}
			DidCrouch.Broadcast(this);
		}
	}
	else
	{
		if (!bToggleCrouch && bCrouching)
		{
			if (OwnerAsChar)
			{
				RestoreLastMovementMode();
				OwnerAsChar->UnCrouch();
				Client_Crouch(false);
				bCrouching = false;
			}
			else
			{
				RestoreLastMovementMode();
				CustomCrouch(bPressed, false);
			}
		}
		if (!bToggleSlide && bSliding)
		{
			if (OwnerAsChar)
			{
				RestoreLastMovementMode();
				bSliding = false;
			}
			else
			{
				RestoreLastMovementMode();
				CustomCrouch(bPressed, true);
			}
		}
	}
}

bool USymplAdvancedMovementComponent::Server_AdvancedCrouch_Validate(bool bPressed) { return true; }

void USymplAdvancedMovementComponent::CustomCrouch_Implementation(bool bPressed, bool bIsSliding)
{
	//Custom crouch.
	CustomCrouch_Impl(bPressed, bIsSliding);
}

void USymplAdvancedMovementComponent::CustomCrouch_Impl(bool bPressed, bool bIsSliding)
{
	//Basic crouch.
	if (bSliding)
	{
		DidSlide.Broadcast(this);
	}
	else
	{
		DidCrouch.Broadcast(this);
	}
}

bool USymplAdvancedMovementComponent::CanSlide()
{
	//Check slide times, angles and velocity.
	if (!bCanSlide && !bProne)
	{
		return false;
	}
	if (MaxSlideTime >= 0.f)
	{
		if (SlideTime >= MaxSlideTime)
		{
			return false;
		}
	}
	double velocity = CurrentVelocity.Size();
	if (velocity >= RequiredSlideSpeed)
	{
		if (bIgnoreSlideAngle)
		{
			return true;
		}
		if (!OwnerAsChar->GetCharacterMovement()->IsFalling())
		{
			double angle = OwnerAsChar->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal.Dot(FVector(0.f, 0.f, 1.f));
			angle = UKismetMathLibrary::DegAcos(angle);
			return angle >= RequiredSlideAngle;
		}
	}
	return false;
}

void USymplAdvancedMovementComponent::Server_Sprint_Implementation(bool bPressed)
{
	if (bPressed)
	{
		bSprinting = !bSprinting;
		Server_SetMovementMode(bSprinting ? EAdvancedMovementMode::ESPRINT : LastMovementMode);
	}
	else
	{
		if (!bToggleSprint)
		{
			RestoreLastMovementMode();
			bSprinting = false;
		}
	}
}

bool USymplAdvancedMovementComponent::Server_Sprint_Validate(bool bPressed) { return true; }

void USymplAdvancedMovementComponent::Server_SetMovementMode_Implementation(EAdvancedMovementMode Mode)
{
	LastMovementMode = CurrentMovementMode;
	CurrentMovementMode = Mode;
	MovementModeUpdate.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetMovementMode_Validate(EAdvancedMovementMode Mode) { return true; }

void USymplAdvancedMovementComponent::Server_SetMovementSpeeds_Implementation(const TArray<FSymplMovementSpeeds>& InSpeeds, EAdvancedMovementMode NewMode, bool bForceSetMovementMode)
{
	SelectedSpeeds = InSpeeds;
	if (bForceSetMovementMode)
	{
		Server_SetMovementMode(NewMode);
	}
	SelectedSpeedsUpdate.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetMovementSpeeds_Validate(const TArray<FSymplMovementSpeeds>& InSpeeds, EAdvancedMovementMode NewMode, bool bForceSetMovementMode) { return true; }

void USymplAdvancedMovementComponent::RestoreLastMovementMode()
{
	Server_SetMovementMode(LastMovementMode);
}

void USymplAdvancedMovementComponent::Server_SetProne_Implementation(bool bPressed)
{
	if (bPressed)
	{
		bProne = !bProne;
		Server_SetMovementMode(bProne ? EAdvancedMovementMode::EPRONE : LastMovementMode);
		if (bProne)
		{
			if (OwnerAsChar)
			{
				// Set the character's capsule height and radius to simulate going prone
				OwnerAsChar->GetCapsuleComponent()->SetCapsuleSize(40.f, 20.f);
				OwnerAsChar->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
			}
		}
	}
	else
	{
		if (!bToggleProne)
		{
			RestoreLastMovementMode();
			bProne = false;
			if (OwnerAsChar)
			{
				// Set the character's capsule height and radius to simulate going prone
				OwnerAsChar->GetCapsuleComponent()->SetCapsuleSize(DefaultCapsuleSize.X,DefaultCapsuleSize.Y);
				OwnerAsChar->GetMesh()->SetRelativeLocation(DefaultMeshLocation);
			}
		}
	}
	DidProne.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetProne_Validate(bool bPressed) { return true; }

void USymplAdvancedMovementComponent::Server_Dash_Implementation(bool bPressed, FVector Direction, bool bForceEnd)
{
	if (bPressed && CanDash())
	{
		bDashing = !bDashing;
		Server_SetMovementMode(bDashing ? EAdvancedMovementMode::EDASH : LastMovementMode);
		CurrentDashDirection = Direction;
	}
	else
	{
		if (!bToggleDash || bForceEnd)
		{
			bDashing = false;
			RestoreLastMovementMode();
		}
	}
	DidDash.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_Dash_Validate(bool bPressed, FVector Direction, bool bForceEnd) { return true; }

void USymplAdvancedMovementComponent::Server_Blink_Implementation(bool bPressed, FVector Direction, bool bForceEnd)
{
	if (bPressed && CanBlink())
	{
		bBlinking = !bBlinking;
		Server_SetMovementMode(bBlinking ? EAdvancedMovementMode::EBLINK : LastMovementMode);
		CurrentBlinkDirection = Direction;
	}
	else
	{
		if (!bToggleBlink || bForceEnd)
		{
			bBlinking = false;
			RestoreLastMovementMode();
		}
	}
	DidBlink.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_Blink_Validate(bool bPressed, FVector Direction, bool bForceEnd) { return true; }

void USymplAdvancedMovementComponent::Server_Roll_Implementation(bool bPressed, FVector Direction, bool bForceEnd)
{
	if (bPressed && CanRoll())
	{
		bRolling = !bRolling;
		Server_SetMovementMode(bRolling ? EAdvancedMovementMode::EROLL : LastMovementMode);
		CurrentRollDirection = Direction;
	}
	else
	{
		if (!bToggleRoll || bForceEnd)
		{
			bRolling = false;
			RestoreLastMovementMode();
		}
	}
	DidRoll.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_Roll_Validate(bool bPressed, FVector Direction, bool bForceEnd) { return true; }

void USymplAdvancedMovementComponent::Server_SetMovementAnimations(FSymplMovementAnimations Animations)
{
	CurrentMovementAnimations = Animations;
	AnimationUpdate.Broadcast(this);
}

//bool USymplAdvancedMovementComponent::Server_SetMovementAnimations_Validate(FSymplMovementAnimations Animations) { return true; }

double USymplAdvancedMovementComponent::ReplicatedMontage(UAnimMontage* Montage, bool bUseAnimInstance, double PlayRate, 
	double StartPosition, bool bStopMontages, FName StartSectionName)
{
	if (!Montage)
	{
		return .1f;
	}
	Server_PlayMontage(Montage, bUseAnimInstance, PlayRate, StartPosition, bStopMontages, StartSectionName);
	return Montage->GetPlayLength() * PlayRate;
}

void USymplAdvancedMovementComponent::Server_PlayMontage_Implementation(UAnimMontage* Montage, bool bUseAnimInstance, double PlayRate, 
	double StartPosition, bool bStopMontages, FName StartSectionName)
{
	Multicast_PlayMontage(Montage, bUseAnimInstance, PlayRate, StartPosition, bStopMontages, StartSectionName);
}

bool USymplAdvancedMovementComponent::Server_PlayMontage_Validate(UAnimMontage* Montage, bool bUseAnimInstance, double PlayRate, 
	double StartPosition, bool bStopMontages, FName StartSectionName) { return true; }

void USymplAdvancedMovementComponent::Multicast_PlayMontage_Implementation(UAnimMontage* Montage, bool bUseAnimInstance, 
	double PlayRate, double StartPosition, bool bStopMontages, FName StartSectionName)
{
	if (!Montage)
	{
		return;
	}
	USkeletalMeshComponent* mesh = Cast<USkeletalMeshComponent>(OwnerRef->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (bUseAnimInstance || !OwnerAsChar)
	{
		if (mesh)
		{
			mesh->GetAnimInstance()->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, StartPosition, bStopMontages);
		}
	}
	else
	{
		OwnerAsChar->PlayAnimMontage(Montage, PlayRate, StartSectionName);
	}
}

double USymplAdvancedMovementComponent::ReplicatedMontage_FromAnimStruct(FSymplMovementAnimation Anim)
{
	UAnimMontage* montage = Cast<UAnimMontage>(Anim.Anim);
	if (!montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to call ReplicatedMontage, but the anim struct %s did not hold a montage!"), *Anim.Name.ToString());
	}
	return ReplicatedMontage(montage, Anim.bStopMontages, Anim.PlayRate, Anim.StartPosition, Anim.bStopMontages, Anim.StartSectionName);
}

void USymplAdvancedMovementComponent::Server_SetHovering_Implementation(bool bPressed, bool bForceEndHover)
{
	if (bPressed)
	{
		bHovering = !bHovering;
		if(bHovering)
		{
			if (OwnerAsChar)
			{
				LastCharacterMovementMode = OwnerAsChar->GetCharacterMovement()->MovementMode;
				OwnerAsChar->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			}
			Server_SetMovementMode(EAdvancedMovementMode::EHOVER);
		}
	}
	else
	{
		if (!bToggleHover || bForceEndHover)
		{
			bHovering = false;
		}
	}
	if (!bHovering)
	{
		HoverTime = 0.f;
		RestoreLastMovementMode();
		RestoreLastCharacterMovementMode();
	}
	DidHover.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetHovering_Validate(bool bPressed, bool bForceEndHover)
{
	return true;
}

void USymplAdvancedMovementComponent::RestoreLastCharacterMovementMode()
{
	if (OwnerAsChar)
	{
		OwnerAsChar->GetCharacterMovement()->SetMovementMode(LastCharacterMovementMode);
	}
}

void USymplAdvancedMovementComponent::Server_DeployParachute_Implementation()
{
	Server_SetMovementMode(EAdvancedMovementMode::EPARACHUTE);
	if (OwnerAsChar)
	{
		LastMaxAcceleration = OwnerAsChar->GetCharacterMovement()->MaxAcceleration;

		// Disable character movement
		OwnerAsChar->GetCharacterMovement()->DisableMovement();

		// Spawn parachute actor and attach it to character
		if (ParachuteClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwnerRef;
			SpawnParams.Instigator = OwnerAsChar;

			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);

			ParachuteActor = GetWorld()->SpawnActor<AActor>(ParachuteClass.LoadSynchronous(), SpawnParams);
			ParachuteActor->AttachToComponent(OwnerAsChar->GetMesh(), AttachRules, ParachuteAttachSocket);
		}

		ReplicatedMontage_FromAnimStruct(CurrentMovementAnimations.ParachuteAnim);

		// Enable parachute control and camera
		bIsParachuting = true;

		// Adjust character movement properties for parachute descent
		OwnerAsChar->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		OwnerAsChar->GetCharacterMovement()->MaxAcceleration = ParachuteMaxAcceleration;
	}
}

bool USymplAdvancedMovementComponent::Server_DeployParachute_Validate()
{
	return true;
}

void USymplAdvancedMovementComponent::Server_ReleaseParachute_Implementation()
{
	RestoreLastCharacterMovementMode();
	RestoreLastMovementMode();
	if (OwnerAsChar)
	{
		// Detach and destroy parachute actor
		if (ParachuteActor)
		{
			ParachuteActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ParachuteActor->Destroy();
			ParachuteActor = nullptr;
		}

		if (OwnerAsChar && bIsParachuting)
		{
			// Disable parachute control
			bIsParachuting = false;

			// Adjust character movement properties back to normal
			OwnerAsChar->GetCharacterMovement()->MaxAcceleration = LastMaxAcceleration;
		}
	}
}

bool USymplAdvancedMovementComponent::Server_ReleaseParachute_Validate(){return true;}

bool USymplAdvancedMovementComponent::CanDeployParachute()
{
	FHitResult hit;
	if (!FindFloor(hit))
		return true;
	return hit.Distance >= RequiredDistanceToDeployParachute;
}

bool USymplAdvancedMovementComponent::FindFloor(FHitResult& OutHit)
{
	if (OwnerAsChar)
	{
		OutHit = OwnerAsChar->GetCharacterMovement()->CurrentFloor.HitResult;
		return OutHit.bBlockingHit;
	}
	else
	{
		FVector loc = OwnerRef->GetActorLocation();
		FVector dir = OwnerRef->GetActorUpVector() * 1.f;
		FCollisionShape shape;
		shape.MakeSphere(1.f);
		FCollisionQueryParams params;
		FCollisionResponseParams response;
		return GetWorld()->SweepSingleByChannel(OutHit, loc, loc + (dir * FloorCheckDistance), FQuat(), FloorTraceChannel, shape, params, response);
	}
}

void USymplAdvancedMovementComponent::Server_SetForwardInput_Implementation(double Value)
{
	ForwardInput = Value;
}

bool USymplAdvancedMovementComponent::Server_SetForwardInput_Validate(double Value) { return true; }

void USymplAdvancedMovementComponent::Server_SetRightInput_Implementation(double Value)
{
	RightInput = Value;
}

bool USymplAdvancedMovementComponent::Server_SetRightInput_Validate(double Value) { return true; }

void USymplAdvancedMovementComponent::Server_SetUpInput_Implementation(double Value)
{
	UpInput = Value;
}

bool USymplAdvancedMovementComponent::Server_SetUpInput_Validate(double Value) { return true; }

void USymplAdvancedMovementComponent::AdvancedMovementInput_Forward(double Value, bool bForce)
{
	if (!CanMove())
	{
		return;
	}
	ForwardInput = Value;
	Server_SetForwardInput(Value);
	if (OwnerAsPawn)
	{
		OwnerAsPawn->AddMovementInput(OwnerRef->GetActorForwardVector(), Value, bForce);
	}
	else
	{
		OwnerRef->AddActorLocalOffset(GetInputDirection());
	}
}

void USymplAdvancedMovementComponent::AdvancedMovementInput_Right(double Value, bool bForce)
{
	if (!CanMove())
	{
		return;
	}
	RightInput = Value;
	Server_SetRightInput(Value);
	if (OwnerAsPawn)
	{
		OwnerAsPawn->AddMovementInput(OwnerRef->GetActorRightVector(), Value, bForce);
	}
	else
	{
		OwnerRef->AddActorLocalOffset(GetInputDirection());
	}
}

void USymplAdvancedMovementComponent::AdvancedMovementInput_Up(double Value, bool bForce)
{
	if (!CanMove())
	{
		return;
	}
	UpInput = Value;
	Server_SetUpInput(Value);
	if (OwnerAsPawn)
	{
		OwnerAsPawn->AddMovementInput(OwnerRef->GetActorUpVector(), Value, bForce);
	}
	else
	{
		OwnerRef->AddActorLocalOffset(GetInputDirection());
	}
}

void USymplAdvancedMovementComponent::Server_SetZeroGMovement_Implementation(bool bZeroG)
{
	bZeroGMovement = bZeroG;
	if (bZeroGMovement)
	{
		Server_SetMovementMode(EAdvancedMovementMode::EZEROG);
	}
	else
	{
		RestoreLastMovementMode();
	}
	DidZeroG.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetZeroGMovement_Validate(bool bZeroG) { return true; }

void USymplAdvancedMovementComponent::Server_SetJetpack_Implementation(bool bPressed)
{
	bJetpackActive = bPressed;
	if (bJetpackActive)
	{
		Server_SetMovementMode(EAdvancedMovementMode::EJETPACK);
	}
	else
	{
		RestoreLastMovementMode();
	}
	DidJetpack.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetJetpack_Validate(bool bPressed) { return true; }

void USymplAdvancedMovementComponent::Server_SetJetpackFuel_Implementation(double Value)
{
	CurrentJetpackFuel = Value;
	JetpackFuelUpdate.Broadcast(this);
}

bool USymplAdvancedMovementComponent::Server_SetJetpackFuel_Validate(double Value) { return true; }

void USymplAdvancedMovementComponent::Server_UpdateTransform_Implementation(FTransform Transform)
{
	OwnerRef->SetActorTransform(Transform);
}

bool USymplAdvancedMovementComponent::Server_UpdateTransform_Validate(FTransform Transform) { return true; }

void USymplAdvancedMovementComponent::MoveToLastGroundLocation()
{
	Server_UpdateTransform(FTransform(OwnerRef->GetActorRotation(), LastGroundLocation, OwnerRef->GetActorScale3D()));
}

void USymplAdvancedMovementComponent::MoveToLastAirLocation()
{
	Server_UpdateTransform(FTransform(OwnerRef->GetActorRotation(), LastAirLocation, OwnerRef->GetActorScale3D()));
}

void USymplAdvancedMovementComponent::Client_Crouch_Implementation(bool bPressed)
{
	if (OwnerAsChar)
	{
		if (bPressed)
		{
			OwnerAsChar->Crouch();
		}
		else
		{
			OwnerAsChar->UnCrouch();
		}
	}
}

void USymplAdvancedMovementComponent::Client_Jump_Implementation(bool bPressed)
{
	if (OwnerAsChar)
	{
		if (bPressed)
		{
			OwnerAsChar->Jump();
		}
		else
		{
			OwnerAsChar->StopJumping();
		}
	}
}
