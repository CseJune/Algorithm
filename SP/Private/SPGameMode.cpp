#include "SPGameMode.h"
#include "SP_Character.h"
#include "SPPlayerController.h"
#include "SPGameState.h"

ASPGameMode::ASPGameMode()
{
	DefaultPawnClass = ASP_Character::StaticClass();
	PlayerControllerClass = ASPPlayerController::StaticClass();
	GameStateClass = ASPGameState::StaticClass();
}