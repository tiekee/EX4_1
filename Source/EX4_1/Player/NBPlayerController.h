#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"
class UNBChatInput;
class UUserWidget;

UCLASS()
class EX4_1_API ANBPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void SetChatMessageString(const FString& Message);
    void SetChatMessageString(const FString& Message, bool bChatOnly);
    UFUNCTION(Server, Reliable) void ServerRPCSubmitMessage(const FString& Message, bool bChatOnly);
    UFUNCTION(Client, Reliable) void ClientRPCPrintChatMessageString(const FString& Message);
    UFUNCTION(Client, Reliable) void ClientRPCResetChat();
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Notification") FText NotificationText;
    UPROPERTY(EditDefaultsOnly, Category="UI") TSubclassOf<UNBChatInput> ChatInputWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category="UI") TSubclassOf<UUserWidget> NotificationTextWidgetClass;
protected:
    UPROPERTY() TObjectPtr<UNBChatInput> ChatInputWidgetInstance;
    UPROPERTY() TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
    FString ChatMessageString;
};
