#pragma once

#include "CoreMinimal.h"
#include "SuperMediaBase.h"
#include "ProjectMediaReceiver.generated.h"

class AStaticMeshActor;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UTexture;

// Project example. These Blueprint nodes/events are project additions.
UCLASS(Blueprintable)
class AProjectMediaReceiver : public ASuperMediaBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Project|Media")
    TObjectPtr<AStaticMeshActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Media")
    TObjectPtr<UMaterialInterface> BaseMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Media", meta=(ClampMin="0"))
    int32 MaterialIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Media")
    FName TextureParameter = TEXT("ProjectMedia");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project|Media")
    TObjectPtr<UTexture> FallbackTexture;

    UFUNCTION(BlueprintPure, Category="Project|Media")
    UTexture* GetProjectTexture() const;

protected:
    virtual void OnActiveTextureChanged() override;

    // Reference-change notification in the running world; not a video-frame event.
    UFUNCTION(BlueprintImplementableEvent, Category="Project|Media")
    void ProjectTextureChanged(UTexture* Texture);

private:
    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> Material;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInterface> ActiveBaseMaterial;
};
