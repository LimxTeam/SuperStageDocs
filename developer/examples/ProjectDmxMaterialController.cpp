#include "ProjectDmxMaterialController.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

bool AProjectDmxMaterialController::EnsureMaterial()
{
    if (!bEnabled || HasAnyFlags(RF_ClassDefaultObject)) return false;
    if (!IsValid(TargetActor.Get()) || !IsValid(BaseMaterial.Get())) return false;
    UStaticMeshComponent* Mesh = TargetActor->GetStaticMeshComponent();
    if (!IsValid(Mesh) || MaterialIndex < 0 || MaterialIndex >= Mesh->GetNumMaterials()) return false;
    if (!IsValid(Material.Get()) || Material->GetOuter() != this
        || ActiveBaseMaterial.Get() != BaseMaterial.Get())
    {
        Material = UMaterialInstanceDynamic::Create(BaseMaterial.Get(), this);
        ActiveBaseMaterial = BaseMaterial;
    }
    if (!IsValid(Material.Get())) return false;
    Mesh->SetMaterial(MaterialIndex, Material.Get());
    return true;
}

void AProjectDmxMaterialController::NativeLightInitialization()
{
    Super::NativeLightInitialization();
    if (EnsureMaterial()) ApplyCachedParameters();
}

void AProjectDmxMaterialController::NativeSuperDMXChanged()
{
    Super::NativeSuperDMXChanged();
    if (ControlMode == ESuperDMXControlMode::Property || !EnsureMaterial()) return;

    // Read the six attributes defined in MaterialControl_6CH.json.
    // Address validation also prevents out-of-range reads from masquerading as zero.
    static const FName Names[] = {TEXT("Red"), TEXT("Green"), TEXT("Blue"),
        TEXT("Effect"), TEXT("Speed"), TEXT("Width")};
    for (const FName Name : Names)
    {
        int32 C = 0, F = 0, U = 0;
        if (!ResolveAttributeAddressesByIndex(0, Name, C, F, U) || C < 1 || C > 512) return;
    }
    GetSuperDMXColorValue(MakeDmx(TEXT("Red")), MakeDmx(TEXT("Green")),
        MakeDmx(TEXT("Blue")), Color);
    GetSuperDmxAttributeValue(MakeDmx(TEXT("Effect")), Effect);
    GetSuperDmxAttributeValue(MakeDmx(TEXT("Speed")), Speed);
    GetSuperDmxAttributeValue(MakeDmx(TEXT("Width")), Width);
    ApplyCachedParameters();
}

void AProjectDmxMaterialController::ApplyCachedParameters()
{
    if (!IsValid(Material.Get())) return;
    Material->SetVectorParameterValue(TEXT("ProjectColor"), Color);
    Material->SetScalarParameterValue(TEXT("ProjectEffect"), FMath::Lerp(0.0f, 10.0f, Effect));
    Material->SetScalarParameterValue(TEXT("ProjectSpeed"), FMath::Lerp(-10.0f, 10.0f, Speed));
    Material->SetScalarParameterValue(TEXT("ProjectWidth"), FMath::Lerp(0.0f, 10.0f, Width));
}
