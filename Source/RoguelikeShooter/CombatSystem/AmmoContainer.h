#pragma once

#include "AmmoContainer.generated.h"

USTRUCT(BlueprintType)
struct FAmmoContainer
{
    GENERATED_BODY()

public:

    FAmmoContainer();
    FAmmoContainer(class UAmmoDefinition* NewAmmoDefinition, int NewAmmoAmount);

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UAmmoDefinition* AmmoDefinition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int AmmoAmount;

    static FAmmoContainer* FindAmmoContainer(TArray<FAmmoContainer>& InArray, class UAmmoDefinition* Definition);
    static const FAmmoContainer* FindAmmoContainer(const TArray<FAmmoContainer>& InArray, class UAmmoDefinition* Definition);

        
};
