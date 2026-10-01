#include "ProjectMediaReceiver.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

UTexture* AProjectMediaReceiver::GetProjectTexture() const
{
    UTexture* Active = GetActiveTexture();
    return IsValid(Active) ? Active : FallbackTexture.Get();
}

void AProjectMediaReceiver::OnActiveTextureChanged()
{
    Super::OnActiveTextureChanged();
    if (HasAnyFlags(RF_ClassDefaultObject)) return;
    UTexture* Texture = GetProjectTexture();
    if (IsValid(TargetActor.Get()) && IsValid(BaseMaterial.Get()))
    {
        UStaticMeshComponent* Mesh = TargetActor->GetStaticMeshComponent();
        if (IsValid(Mesh) && MaterialIndex >= 0 && MaterialIndex < Mesh->GetNumMaterials())
        {
            if (!IsValid(Material.Get()) || Material->GetOuter() != this
                || ActiveBaseMaterial.Get() != BaseMaterial.Get())
            {
                Material = UMaterialInstanceDynamic::Create(BaseMaterial.Get(), this);
                ActiveBaseMaterial = BaseMaterial;
            }
            if (IsValid(Material.Get()))
            {
                Mesh->SetMaterial(MaterialIndex, Material.Get());
                Material->SetTextureParameterValue(TextureParameter, Texture);
            }
        }
    }
    // Parent schedules a deferred initial notification after initialization.
    if (GetWorld() && GetWorld()->IsGameWorld() && HasActorBegunPlay())
    {
        ProjectTextureChanged(Texture);
    }
}
