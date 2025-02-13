#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SP_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class SP_API ASP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ASP_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DamageTextClass;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount, // 데미지 양
		struct FDamageEvent const& DamageEvent, // 데미지를 받을 유형, 추가적인 정보를 받을 수 있음, 스킬시스템 같은(불,물속성)
		AController* EventInstigator, // 데미지를 누가 입혔는지(지금은 쓸일이 없다)
		AActor* DamageCauser) override; // 데미지를 일으킨 오브젝트(현 프로젝트에선 MineItem)

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void OnDeath();
	void UpdateOverheadHp();


private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	void ShowDamageText(float DamageAmount);
};