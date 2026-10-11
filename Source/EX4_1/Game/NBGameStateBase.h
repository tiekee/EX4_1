#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBGameStateBase.generated.h"

UCLASS()
class EX4_1_API ANBGameStateBase : public AGameStateBase
{
    GENERATED_BODY()
public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    UPROPERTY(Replicated, BlueprintReadOnly) FString NotificationText;
    UPROPERTY(Replicated, BlueprintReadOnly) bool bRoundFinished = false;
};
