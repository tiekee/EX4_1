#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"
class ANBPlayerController;

UCLASS()
class EX4_1_API ANBGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ANBGameModeBase();
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;
    static FString GenerateSecretNumber();
    static bool IsGuessNumberString(const FString& Number);
    static FString JudgeResult(const FString& Secret, const FString& Guess);
    void PrintChatMessageString(ANBPlayerController* Sender, const FString& Message, bool bChatOnly);
    void JudgeGameResult(ANBPlayerController* Sender, const FString& Result);
    void ResetGame();
private:
    friend class FNBGameFlowTest;
    void BroadcastChat(const FString& Message);
    void FinishRound(const FString& Notice);
    FString SecretNumberString;
    UPROPERTY() TArray<TObjectPtr<ANBPlayerController>> AllPlayerControllers;
    int32 NextPlayerNumber = 1;
    FTimerHandle ResetTimer;
};
