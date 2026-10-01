#pragma once

#include "CoreMinimal.h"
#include "SuperDmxActorBase.h"
#include "ProjectDmxMaterialController.generated.h"

class AStaticMeshActor;
class UMaterialInterface;
class UMaterialInstanceDynamic;

// Project example: material initialization and DMX parameter changes.
UCLASS(Blueprintable)
class AProjectDmxMaterialController : public ASuperDmxActorBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Project|Material")
    TObjectPtr<AStaticMeshActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Material")
    TObjectPtr<UMaterialInterface> BaseMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Material", meta=(ClampMin="0"))
    int32 MaterialIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Material")
    bool bEnabled = false;

protected:
    virtual void NativeLightInitialization() override;
    virtual void NativeSuperDMXChanged() override;

private:
    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> Material;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInterface> ActiveBaseMaterial;

    FLinearColor Color = FLinearColor::White;
    float Effect = 0.0f;
    float Speed = 0.5f;
    float Width = 0.0f;
    bool EnsureMaterial();
    void ApplyCachedParameters();
};
