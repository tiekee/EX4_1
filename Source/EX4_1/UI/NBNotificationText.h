#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBNotificationText.generated.h"
UCLASS()
class EX4_1_API UNBNotificationText : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="Notification") FText GetNotificationText() const;
};
