#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkObject.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API UNetworkObject : public UObject
{
	GENERATED_BODY()

public:

	UNetworkObject();

	virtual bool IsSupportedForNetworking() const override { return true; };

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags);
};