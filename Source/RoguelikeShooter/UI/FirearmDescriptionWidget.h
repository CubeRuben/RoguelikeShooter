#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirearmDescriptionWidget.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API UFirearmDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void InitFirearm(class UFirearm* Firearm);

	UFUNCTION(BlueprintImplementableEvent)
	void StartHovering();

	UFUNCTION(BlueprintImplementableEvent)
	void StopHovering();	
};
