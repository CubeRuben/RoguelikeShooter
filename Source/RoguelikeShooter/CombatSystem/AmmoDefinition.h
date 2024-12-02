#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AmmoDefinition.generated.h"

UCLASS(BlueprintType)
class ROGUELIKESHOOTER_API UAmmoDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UAmmoDefinition();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmount;

public:

	FString GetName() const { return Name; }
	int GetMaxAmount() const { return MaxAmount; }
};
