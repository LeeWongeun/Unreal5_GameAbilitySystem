// Copyright LeeWongeun

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	//이게 실행되면 컨트롤러가 정상적으로 Set 되었음을 알 수 있다.
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
