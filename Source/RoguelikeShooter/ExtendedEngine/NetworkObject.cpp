#include "NetworkObject.h"

UNetworkObject::UNetworkObject()
{

}

bool UNetworkObject::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	return false;
}
