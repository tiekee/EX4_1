#include "NBGameStateBase.h"
#include "Net/UnrealNetwork.h"
void ANBGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANBGameStateBase, NotificationText);
    DOREPLIFETIME(ANBGameStateBase, bRoundFinished);
}
