#include "AmmoContainer.h"

FAmmoContainer::FAmmoContainer()
{
	AmmoDefinition = nullptr;
	AmmoAmount = 0;
}

FAmmoContainer::FAmmoContainer(UAmmoDefinition* NewAmmoDefinition, int NewAmmoAmount)
{
	AmmoDefinition = NewAmmoDefinition;
	AmmoAmount = NewAmmoAmount;
}

FAmmoContainer* FAmmoContainer::FindAmmoContainer(TArray<FAmmoContainer>& InArray, UAmmoDefinition* Definition)
{
	for (FAmmoContainer& container : InArray) 
	{
		if (container.AmmoDefinition == Definition)
			return &container;
	}

	return nullptr;
}

const FAmmoContainer* FAmmoContainer::FindAmmoContainer(const TArray<FAmmoContainer>& InArray, UAmmoDefinition* Definition)
{
	for (const FAmmoContainer& container : InArray)
	{
		if (container.AmmoDefinition == Definition)
			return &container;
	}

	return nullptr;
}
