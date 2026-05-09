#include "Drone.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NPC.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"

ADrone::ADrone()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();

    if (!TargetNPC)
    {
        TArray<AActor*> FoundNPCs;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), FoundNPCs);
        if (FoundNPCs.Num() > 0)
        {
            TargetNPC = Cast<ANPC>(FoundNPCs[0]);
        }
    }

    GetWorldTimerManager().SetTimer(ScreenshotTimerHandle, this, &ADrone::TakeScreenshot, 1.0f, true);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        SetAsViewTarget(PC);
    }
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateCameraTransform(DeltaTime);
}

void ADrone::UpdateCameraTransform(float DeltaTime)
{
    if (!TargetNPC)
    {
        return;
    }

    CurrentAngleDegrees += OrbitSpeed * DeltaTime;
    if (CurrentAngleDegrees >= 360.0f)
    {
        CurrentAngleDegrees -= 360.0f;
    }

    float Radians = FMath::DegreesToRadians(CurrentAngleDegrees);
    float X = TargetNPC->GetActorLocation().X + OrbitRadius * FMath::Cos(Radians);
    float Y = TargetNPC->GetActorLocation().Y + OrbitRadius * FMath::Sin(Radians);
    float Z = TargetNPC->GetActorLocation().Z + OrbitHeight;

    FVector NewLocation = FVector(X, Y, Z);
    SetActorLocation(NewLocation);

    FVector Direction = TargetNPC->GetActorLocation() - NewLocation;
    FRotator NewRotation = Direction.Rotation();
    SetActorRotation(NewRotation);
}

void ADrone::SetTarget(ANPC* NewTarget)
{
    TargetNPC = NewTarget;
}

void ADrone::SetAsViewTarget(APlayerController* PC)
{
    if (PC)
    {
        PC->SetViewTarget(this);
    }
}

void ADrone::TakeScreenshot()
{
    if (!GetWorld())
    {
        return;
    }

    FDateTime Now = FDateTime::Now();
    FString Timestamp = Now.ToString(TEXT("%Y%m%d_%H%M%S"));
    FString ScreenshotName = FString::Printf(TEXT("DroneScreenshot_%s.png"), *Timestamp);
    
    FString ScreenShotDir = FPaths::ProjectSavedDir() / TEXT("Screenshots/");
    IFileManager::Get().MakeDirectory(*ScreenShotDir, true);
    
    FString FullPath = ScreenShotDir + ScreenshotName;
    
    FScreenshotRequest::RequestScreenshot(FullPath, false, false);
    
    UE_LOG(LogTemp, Warning, TEXT("Screenshot saved: %s"), *FullPath);
}