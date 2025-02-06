// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGameMode.h"
#include "SP_Character.h"
#include "SPPlayerController.h"

ASPGameMode::ASPGameMode()
{
	DefaultPawnClass = ASP_Character::StaticClass();
	PlayerControllerClass = ASPPlayerController::StaticClass();
}