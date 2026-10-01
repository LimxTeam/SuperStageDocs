#pragma once

#include "CoreMinimal.h"
#include "SuperDmxActorBase.h"
#include "ProjectDmxMotionController.generated.h"

// Project example: continuous DMX motion through the Super event only.
UCLASS(Blueprintable)
class AProjectDmxMotionController : public ASuperDmxActorBase
{
    GENERATED_BODY()
public:
    AProjectDmxMotionController();

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Project|Motion")
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion")
    FSuperDMXAttribute PositionInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion")
    FVector StartPosition = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion")
    FVector EndPosition = FVector(0, 0, 300);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion")
    bool bEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion")
    bool bPreviewInEditor = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion")
    bool bInterpolate = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Motion", meta=(ClampMin="0"))
    float InterpSpeed = 5.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Project|Motion")
    float PositionAlpha = 0.0f;

protected:
    virtual void NativeLightInitialization() override;
    virtual void NativeSuperDMXTick() override;

private:
    void ReadDMXAndApply();
};
