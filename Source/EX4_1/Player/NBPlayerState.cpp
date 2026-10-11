#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANBPlayerState, CurrentGuessCount);
    DOREPLIFETIME(ANBPlayerState, MaxGuessCount);
}
FString ANBPlayerState::GetPlayerInfoString() const
{
    return FString::Printf(TEXT("%s(%d/%d)"), *GetPlayerName(), CurrentGuessCount, MaxGuessCount);
}
void ANBPlayerState::IncreaseGuessCount()
{
    if (HasAuthority() && CurrentGuessCount < MaxGuessCount) { ++CurrentGuessCount; ForceNetUpdate(); }
}
void ANBPlayerState::ResetGuessCount()
{
    if (HasAuthority()) { CurrentGuessCount = 0; ForceNetUpdate(); }
}
