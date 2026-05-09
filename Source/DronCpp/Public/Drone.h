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

    UPROPERTY(EditAnywhere, Category = "Screenshot Settings")
    bool bEnableScreenshots = true;

    UPROPERTY(EditAnywhere, Category = "Screenshot Settings", meta = (EditCondition = "bEnableScreenshots"))
    int32 ScreenshotWidth = 640;
    
    UPROPERTY(EditAnywhere, Category = "Screenshot Settings", meta = (EditCondition = "bEnableScreenshots"))
    int32 ScreenshotHeight = 480;
    
    UPROPERTY(EditAnywhere, Category = "Screenshot Settings", meta = (EditCondition = "bEnableScreenshots", ClampMin = "0.1", ClampMax = "10.0"))
    float ScreenshotInterval = 1.0f;

    float CurrentAngleDegrees = 0.0f;

    UPROPERTY()
    class ANPC* TargetNPC;

    void FindNearestNPC();
    void UpdateDronePosition(float DeltaTime);
    void LookAtNPC();
    void TakeScreenshot();

    FTimerHandle ScreenshotTimerHandle;
    FTimerHandle FindNPCTimerHandle;
    
    int32 ScreenshotCounter = 0;
};