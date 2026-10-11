#include "NBPlayerController.h"
#include "../Game/NBGameModeBase.h"
#include "../UI/NBChatInput.h"
#include "Components/EditableTextBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void ANBPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (!IsLocalController()) return;
    FInputModeUIOnly InputMode;
    if (ChatInputWidgetClass)
    {
        ChatInputWidgetInstance = CreateWidget<UNBChatInput>(this, ChatInputWidgetClass);
        if (ChatInputWidgetInstance)
        {
            ChatInputWidgetInstance->AddToViewport();
            if (ChatInputWidgetInstance->EditableTextBox_ChatInput)
                InputMode.SetWidgetToFocus(ChatInputWidgetInstance->EditableTextBox_ChatInput->TakeWidget());
        }
    }
    if (NotificationTextWidgetClass)
    {
        NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
        if (NotificationTextWidgetInstance) NotificationTextWidgetInstance->AddToViewport(1);
    }
    bShowMouseCursor = true;
    SetInputMode(InputMode);
}
void ANBPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANBPlayerController, NotificationText);
}
void ANBPlayerController::SetChatMessageString(const FString& Message)
{
    bool bGuess = Message.Len() == 3 || Message.IsEmpty();
    for (TCHAR C : Message) if (FChar::IsDigit(C)) bGuess = true;
    SetChatMessageString(Message, !bGuess);
}
void ANBPlayerController::SetChatMessageString(const FString& Message, bool bChatOnly)
{
    ChatMessageString = Message;
    if (IsLocalController()) ServerRPCSubmitMessage(ChatMessageString, bChatOnly);
}
void ANBPlayerController::ServerRPCSubmitMessage_Implementation(const FString& Message, bool bChatOnly)
{
    if (ANBGameModeBase* GM = GetWorld()->GetAuthGameMode<ANBGameModeBase>())
        GM->PrintChatMessageString(this, Message, bChatOnly);
}
void ANBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& Message)
{
    UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(0.f, 0.66f, 1.f), 5.f);
}
void ANBPlayerController::ClientRPCResetChat_Implementation()
{
    if (ChatInputWidgetInstance && ChatInputWidgetInstance->EditableTextBox_ChatInput)
        ChatInputWidgetInstance->EditableTextBox_ChatInput->SetText(FText::GetEmpty());
}
