#include "NBChatInput.h"
#include "Components/EditableTextBox.h"
#include "../Player/NBPlayerController.h"
void UNBChatInput::NativeConstruct()
{
    Super::NativeConstruct();
    if (EditableTextBox_ChatInput)
        EditableTextBox_ChatInput->OnTextCommitted.AddUniqueDynamic(this, &ThisClass::OnChatInputTextCommitted);
}
void UNBChatInput::NativeDestruct()
{
    if (EditableTextBox_ChatInput)
        EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
    Super::NativeDestruct();
}
void UNBChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod != ETextCommit::OnEnter) return;
    if (ANBPlayerController* PC = Cast<ANBPlayerController>(GetOwningPlayer()))
    {
        PC->SetChatMessageString(Text.ToString());
        EditableTextBox_ChatInput->SetText(FText::GetEmpty());
    }
}
