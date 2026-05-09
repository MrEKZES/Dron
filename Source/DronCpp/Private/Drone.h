#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Drone.generated.h"

UCLASS()
class DRONCPP_API ADrone : public AActor
{
	GENERATED_BODY()

public:
	ADrone();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetTarget(class ANPC* NewTarget);
	void SetAsViewTarget(APlayerController* PC);

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Drone Settings")
	float OrbitRadius = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Settings")
	float OrbitHeight = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Settings")
	float OrbitSpeed = 30.0f;

	float CurrentAngleDegrees = 0.0f;

	UPROPERTY()
	class ANPC* TargetNPC;

	void UpdateCameraTransform(float DeltaTime);
	void TakeScreenshot();

	FTimerHandle ScreenshotTimerHandle;
};