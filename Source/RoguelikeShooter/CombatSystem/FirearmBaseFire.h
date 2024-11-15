#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FirearmBaseFire.generated.h"

UCLASS(EditInlineNew)
class ROGUELIKESHOOTER_API UFirearmBaseFire : public UObject
{
	GENERATED_BODY()
	
public:

	UFirearmBaseFire();

	virtual void OnFire(class UFirearm* Firearm);
};
