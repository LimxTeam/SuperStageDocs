#include "ProjectDmxMotionController.h"
#include "Engine/World.h"

AProjectDmxMotionController::AProjectDmxMotionController()
{
    PositionInput = MakeDmx(TEXT("ZPos"), true);
}

void AProjectDmxMotionController::NativeLightInitialization()
{
    Super::NativeLightInitialization();
    // StartPosition and EndPosition are explicit world-space configuration.
    // Repeated construction must not capture the moved target as a new origin.
    PositionAlpha = FMath::Clamp(PositionAlpha, 0.0f, 1.0f);
}

void AProjectDmxMotionController::NativeSuperDMXTick()
{
    Super::NativeSuperDMXTick();
    ReadDMXAndApply();
}

void AProjectDmxMotionController::ReadDMXAndApply()
{
    if (!bEnabled || ControlMode == ESuperDMXControlMode::Property) return;
    if (!IsValid(TargetActor.Get()) || TargetActor.Get() == this) return;
    UWorld* World = GetWorld();
    if (!World) return;
    if (World->IsGameWorld())
    {
        if (!HasActorBegunPlay()) return;
    }
    else if (!bPreviewInEditor) return;

    // This example deliberately accepts a complete 16-bit protocol only.
    if (PositionInput.DMXChannelType != EDMXChannelType::Fine) return;
    int32 C = 0, F = 0, U = 0;
    if (!ResolveAttributeAddressesByIndex(PositionInput.InstanceIndex,
        PositionInput.AttribName, C, F, U)) return;
    if (C < 1 || C > 512 || F < 1 || F > 512) return;

    // Same read/map/apply pattern as the existing machinery implementation.
    // With no snapshot, the reference keeps its prior value (initially zero).
    GetSuperDmxAttributeValue(PositionInput, PositionAlpha);
    PositionAlpha = FMath::Clamp(PositionAlpha, 0.0f, 1.0f);
    const FVector Desired = FMath::Lerp(StartPosition, EndPosition, PositionAlpha);
    const FVector Applied = bInterpolate
        ? FMath::VInterpTo(TargetActor->GetActorLocation(), Desired,
            World->GetDeltaSeconds(), InterpSpeed)
        : Desired;
    TargetActor->SetActorLocation(Applied);
}
