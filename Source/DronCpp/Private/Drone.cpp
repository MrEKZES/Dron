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
    PrimaryActorTick.TickInterval = 0.033f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;
    
    CameraComponent->bConstrainAspectRatio = true;
    CameraComponent->AspectRatio = 1.333f;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();

    //Поиск NPC с задержкой (как вылюбите говорить Алексей "Даю время движку просраться")
    FTimerHandle StartDelayHandle;
    GetWorldTimerManager().SetTimer(StartDelayHandle, [this]()
    {
        FindNearestNPC();
        GetWorldTimerManager().SetTimer(FindNPCTimerHandle, this, &ADrone::FindNearestNPC, 5.0f, true);
    }, 1.0f, false);

    //Установка камеры как ViewTarget
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        SetAsViewTarget(PC);
    }
    
    if (bEnableScreenshots)
    {
        GetWorldTimerManager().SetTimer(ScreenshotTimerHandle, this, &ADrone::TakeScreenshot, ScreenshotInterval, true, 2.0f);
        UE_LOG(LogTemp, Warning, TEXT("Screenshots ENABLED, taking every %.1f seconds"), ScreenshotInterval);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Screenshots DISABLED"));
    }
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (TargetNPC && IsValid(TargetNPC))
    {
        UpdateDronePosition(DeltaTime);
        LookAtNPC();
    }
}
//Поиск NPC
void ADrone::FindNearestNPC()
{
    if (TargetNPC && IsValid(TargetNPC))
    {
        return;
    }
    
    TArray<AActor*> FoundNPCs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), FoundNPCs);
    
    if (FoundNPCs.Num() > 0)
    {
        TargetNPC = Cast<ANPC>(FoundNPCs[0]);
        CurrentAngleDegrees = 0.0f;
    }
}
// Движение дрона
void ADrone::UpdateDronePosition(float DeltaTime)
{
    if (!TargetNPC) return;

    CurrentAngleDegrees += OrbitSpeed * DeltaTime;
    if (CurrentAngleDegrees >= 360.0f)
    {
        CurrentAngleDegrees -= 360.0f;
    }

    float Radians = FMath::DegreesToRadians(CurrentAngleDegrees);
    FVector CenterPoint = TargetNPC->GetActorLocation();
    
    float X = CenterPoint.X + OrbitRadius * FMath::Cos(Radians);
    float Y = CenterPoint.Y + OrbitRadius * FMath::Sin(Radians);
    float Z = CenterPoint.Z + OrbitHeight;
    
    SetActorLocation(FVector(X, Y, Z));
}
//Захват NPC в ViewTarget
void ADrone::LookAtNPC()
{
    if (!TargetNPC) return;
    
    FVector Direction = TargetNPC->GetActorLocation() - GetActorLocation();
    SetActorRotation(Direction.Rotation());
}

void ADrone::SetAsViewTarget(APlayerController* PC)
{
    if (PC)
    {
        PC->SetViewTarget(this);
    }
}
//Скрины
void ADrone::TakeScreenshot()
{
    if (!bEnableScreenshots)
    {
        return;
    }
    
    if (!TargetNPC || !GetWorld() || !GetWorld()->GetGameViewport())
    {
        return;
    }
    
    if (ScreenshotCounter >= 100)
    {
        GetWorldTimerManager().ClearTimer(ScreenshotTimerHandle);
        UE_LOG(LogTemp, Warning, TEXT("Stopped screenshots after 100 shots"));
        return;
    }
    
    FString ScreenShotDir = FPaths::ProjectSavedDir() / TEXT("Screenshots/");
    IFileManager::Get().MakeDirectory(*ScreenShotDir, true);
    
    FDateTime Now = FDateTime::Now();
    FString Timestamp = Now.ToString(TEXT("%Y%m%d_%H%M%S"));
    FString ScreenshotName = FString::Printf(TEXT("Shot_%dx%d_%s.png"), ScreenshotWidth, ScreenshotHeight, *Timestamp);
    FString FullPath = ScreenShotDir + ScreenshotName;
    
    FScreenshotRequest::RequestScreenshot(FullPath, false, false);
    
    ScreenshotCounter++;
    UE_LOG(LogTemp, Verbose, TEXT("Screenshot %d taken"), ScreenshotCounter);
}