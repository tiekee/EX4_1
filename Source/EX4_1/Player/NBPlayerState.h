#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

UCLASS()
class EX4_1_API ANBPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    UFUNCTION(BlueprintPure) int32 GetCurrentGuessCount() const { return CurrentGuessCount; }
    UFUNCTION(BlueprintPure) int32 GetMaxGuessCount() const { return MaxGuessCount; }
    UFUNCTION(BlueprintPure) FString GetPlayerInfoString() const;
    void IncreaseGuessCount();
    void ResetGuessCount();
private:
    UPROPERTY(Replicated) int32 CurrentGuessCount = 0;
    UPROPERTY(Replicated) int32 MaxGuessCount = 3;
};
