// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possessing a Tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller possessing: %s"), *ControlledTank->GetName());
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimToCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimToCrosshair()
{
	if (!GetControlledTank())
	{
		return;
	}

	FVector HitLocation; //out parameter
	if (GetSightRayHitLocation(HitLocation)) //Has effect that it ray traces
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());
	}
	//if it hits landscape
		//TODO tell tank to aim at this point
}

//Get world location of linetrace through crosshair, true if hit lands
bool ATankPlayerController::GetSightRayHitLocation(FVector & OutHitLocation) const
{
	//find crosshair pos in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto CrossLocation = FVector2D(ViewportSizeX * CrossX, ViewportSizeY * CrossY);
	//de-project the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(CrossLocation, LookDirection))
	{
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}

	// linetrace along the look direction, and see what we hit (up to max  range)



	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D CrossLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; //TOBE Discarded

	return DeprojectScreenPositionToWorld(
		CrossLocation.X,
		CrossLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}