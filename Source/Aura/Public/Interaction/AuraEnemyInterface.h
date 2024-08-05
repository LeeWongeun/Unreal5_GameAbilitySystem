// Copyright LeeWongeun

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraEnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAuraEnemyInterface : public UInterface
{
	GENERATED_BODY()
};


class AURA_API IAuraEnemyInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;

};
