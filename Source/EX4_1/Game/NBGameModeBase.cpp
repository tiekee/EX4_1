#include "NBGameModeBase.h"
#include "NBGameStateBase.h"
#include "../Player/NBPlayerController.h"
#include "../Player/NBPlayerState.h"
#include "TimerManager.h"

ANBGameModeBase::ANBGameModeBase()
{
    PlayerControllerClass = ANBPlayerController::StaticClass();
    PlayerStateClass = ANBPlayerState::StaticClass();
    GameStateClass = ANBGameStateBase::StaticClass();
    DefaultPawnClass = nullptr;
}
void ANBGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    ResetGame();
}
void ANBGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (ANBPlayerController* PC = Cast<ANBPlayerController>(NewPlayer))
    {
        AllPlayerControllers.Add(PC);
        PC->NotificationText = FText::FromString(TEXT("Connected to the game server."));
        if (ANBPlayerState* PS = PC->GetPlayerState<ANBPlayerState>())
        {
            PS->SetPlayerName(FString::Printf(TEXT("Player%d"), NextPlayerNumber++));
            BroadcastChat(PS->GetPlayerName() + TEXT(" 님이 접속했습니다."));
        }
    }
}
void ANBGameModeBase::Logout(AController* Exiting)
{
    AllPlayerControllers.Remove(Cast<ANBPlayerController>(Exiting));
    Super::Logout(Exiting);
    JudgeGameResult(nullptr, FString());
}
FString ANBGameModeBase::GenerateSecretNumber()
{
    TArray<int32> Numbers {1,2,3,4,5,6,7,8,9};
    FString Result;
    for (int32 i = 0; i < 3; ++i)
    {
        const int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
        Result += FString::FromInt(Numbers[Index]);
        Numbers.RemoveAt(Index);
    }
    return Result;
}
bool ANBGameModeBase::IsGuessNumberString(const FString& Number)
{
    if (Number.Len() != 3) return false;
    TSet<TCHAR> Digits;
    for (TCHAR Digit : Number)
    {
        if (Digit < TEXT('1') || Digit > TEXT('9') || Digits.Contains(Digit)) return false;
        Digits.Add(Digit);
    }
    return true;
}
FString ANBGameModeBase::JudgeResult(const FString& Secret, const FString& Guess)
{
    if (!IsGuessNumberString(Secret) || !IsGuessNumberString(Guess)) return TEXT("INVALID");
    int32 Strikes = 0, Balls = 0;
    for (int32 i = 0; i < 3; ++i)
    {
        if (Secret[i] == Guess[i]) ++Strikes;
        else if (Secret.Contains(FString::Chr(Guess[i]))) ++Balls;
    }
    return Strikes == 0 && Balls == 0 ? TEXT("OUT") : FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
}
void ANBGameModeBase::PrintChatMessageString(ANBPlayerController* Sender, const FString& Message, bool bChatOnly)
{
    if (!IsValid(Sender) || !AllPlayerControllers.Contains(Sender)) return;
    ANBPlayerState* PS = Sender->GetPlayerState<ANBPlayerState>();
    ANBGameStateBase* GS = GetGameState<ANBGameStateBase>();
    if (!PS || !GS) return;
    // 일반 채팅과 숫자 제출을 명시적으로 구분하여 잘못된 입력이 채팅으로 빠지지 않게 한다.
    if (bChatOnly)
    {
        const FString Chat = Message.Left(200).TrimStartAndEnd();
        if (!Chat.IsEmpty()) BroadcastChat(PS->GetPlayerName() + TEXT(": ") + Chat);
        return;
    }
    if (GS->bRoundFinished)
    {
        Sender->ClientRPCPrintChatMessageString(TEXT("결과 공지 중입니다. 새 게임을 기다려 주세요."));
        return;
    }
    if (!IsGuessNumberString(Message))
    {
        Sender->ClientRPCPrintChatMessageString(TEXT("다시 입력하세요. 1~9의 서로 다른 숫자 3자리입니다. (기회 소모 없음)"));
        return;
    }
    if (PS->GetCurrentGuessCount() >= PS->GetMaxGuessCount())
    {
        Sender->ClientRPCPrintChatMessageString(TEXT("기회를 모두 사용했습니다. 다른 플레이어를 기다려 주세요."));
        return;
    }
    const FString Result = JudgeResult(SecretNumberString, Message);
    PS->IncreaseGuessCount();
    BroadcastChat(PS->GetPlayerInfoString() + TEXT(": ") + Message + TEXT(" -> ") + Result);
    JudgeGameResult(Sender, Result);
}
void ANBGameModeBase::BroadcastChat(const FString& Message)
{
    for (ANBPlayerController* PC : AllPlayerControllers)
        if (IsValid(PC)) PC->ClientRPCPrintChatMessageString(Message);
}
void ANBGameModeBase::JudgeGameResult(ANBPlayerController* Sender, const FString& Result)
{
    ANBGameStateBase* GS = GetGameState<ANBGameStateBase>();
    if (!GS || GS->bRoundFinished) return;
    if (Sender && Result == TEXT("3S0B"))
    {
        FinishRound(Sender->GetPlayerState<ANBPlayerState>()->GetPlayerName() + TEXT(" has won the game."));
        return;
    }
    if (AllPlayerControllers.IsEmpty()) return;
    for (ANBPlayerController* PC : AllPlayerControllers)
    {
        const ANBPlayerState* PS = IsValid(PC) ? PC->GetPlayerState<ANBPlayerState>() : nullptr;
        if (!PS || PS->GetCurrentGuessCount() < PS->GetMaxGuessCount()) return;
    }
    FinishRound(TEXT("Draw..."));
}
void ANBGameModeBase::FinishRound(const FString& Notice)
{
    ANBGameStateBase* GS = GetGameState<ANBGameStateBase>();
    GS->bRoundFinished = true;
    GS->NotificationText = Notice;
    for (ANBPlayerController* PC : AllPlayerControllers)
        if (IsValid(PC)) { PC->NotificationText = FText::FromString(Notice); PC->ForceNetUpdate(); }
    GS->ForceNetUpdate();
    GetWorldTimerManager().SetTimer(ResetTimer, this, &ANBGameModeBase::ResetGame, 3.f, false);
}
void ANBGameModeBase::ResetGame()
{
    SecretNumberString = GenerateSecretNumber();
    for (ANBPlayerController* PC : AllPlayerControllers)
    {
        if (!IsValid(PC)) continue;
        if (ANBPlayerState* PS = PC->GetPlayerState<ANBPlayerState>()) PS->ResetGuessCount();
        PC->ClientRPCResetChat();
    }
    if (ANBGameStateBase* GS = GetGameState<ANBGameStateBase>())
    {
        GS->bRoundFinished = false;
        if (GS->NotificationText.IsEmpty()) GS->NotificationText = TEXT("Connected to the game server.");
        GS->ForceNetUpdate();
    }
}
