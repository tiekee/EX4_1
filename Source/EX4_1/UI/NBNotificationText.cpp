#include "NBNotificationText.h"
#include "../Player/NBPlayerController.h"
FText UNBNotificationText::GetNotificationText() const
{
    const ANBPlayerController* PC = Cast<ANBPlayerController>(GetOwningPlayer());
    return PC ? PC->NotificationText : FText::GetEmpty();
}
