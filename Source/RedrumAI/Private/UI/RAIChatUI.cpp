// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatUI.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "GameMode/RAIPlayerController.h"


void URAIChatUI::NativeConstruct()
{
	Super::NativeConstruct();

	RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());

	AIChat = Cast<UTextBlock>(GetWidgetFromName(TEXT("AIChat")));
	UserChat = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserChat")));
	LoadingImage = Cast<UImage>(GetWidgetFromName(TEXT("LoadingImage")));

	if (UserChat)
	{
		//bUserChat->OnTextCommitted.RemoveDynamic(this, &URAIChatUI::OnCommittedText); // 중복 제거
		UserChat->OnTextCommitted.AddDynamic(this, &URAIChatUI::OnCommittedText);
	}
}

void URAIChatUI::NativeDestruct()
{
	if (UserChat)
	{
		UserChat->OnTextCommitted.RemoveDynamic(this, &URAIChatUI::OnCommittedText);
	}

	Super::NativeDestruct();
}

void URAIChatUI::SubmitExternalMessage(FText Text)
{
	OnCommittedText(Text, ETextCommit::OnEnter);
}

void URAIChatUI::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		//TODO:: 1. UserChat의 내용 GM에게 전달. 그 후는 GM이 알아서 처리
		AskSuspect(Text);
		//2. UserChat이 지워질것
		UserChat->SetText(FText::FromString(TEXT("")));
		//3. 이전 AIChat을 지우고 LoadingImage활성화 -> 추후 AIChat이 들어올때 LoadingImage비활성화
		SetAIChat((TEXT("")));
		LoadingImage->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	default:
		break;
	}
}
void URAIChatUI::SetAIChat(FString String)
{
	LoadingImage->SetVisibility(ESlateVisibility::Collapsed);
	AIChat->SetText(FText::FromString(String));
	//나중에 타이핑 애니메이션을 넣어보면 어떨까? 타이머를 통해서 목표 문장까지 한글자씩 채워지는 배열문장을 사용하면 될 것 같다.
}

void URAIChatUI::AskSuspect(FText Text)
{
	RAIPlayerController->AskSuspect(Text);
}

void URAIChatUI::OnClosed()
{
	Super::OnClosed();

	RAIPlayerController->SetTalkingState(false);
}
