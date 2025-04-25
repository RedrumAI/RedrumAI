// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatUI.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CircularThrobber.h"
#include "GameMode/RAIPlayerController.h"


void URAIChatUI::NativeConstruct()
{
	RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());

	AIChat = Cast<UTextBlock>(GetWidgetFromName(TEXT("AIChat")));
	UserChat = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserChat")));
	LoadingCircle = Cast<UCircularThrobber>(GetWidgetFromName(TEXT("LoadingCircle")));

	if (UserChat)
	{
		UserChat->OnTextCommitted.AddDynamic(this, &URAIChatUI::OnCommittedText);
	}
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
		//3. 이전 AIChat을 지우고 LoadingCircle활성화 -> 추후 AIChat이 들어올때 LoadingCircle비활성화
		SetAIChat((TEXT("")));
		LoadingCircle->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	case ETextCommit::OnCleared:
		UserChat->SetUserFocus(GetOwningPlayer());
		break;

	default:
		break;
	}
}
void URAIChatUI::SetAIChat(FString String)
{
	LoadingCircle->SetVisibility(ESlateVisibility::Collapsed);
	AIChat->SetText(FText::FromString(String));
	//나중에 타이핑 애니메이션을 넣어보면 어떨까? 타이머를 통해서 목표 문장까지 한글자씩 채워지는 배열문장을 사용하면 될 것 같다.
}

//엔터를 누른다 -> HUD에서 함수(ChatUI작동)호출 -> ChatUI::텍스트를 GM으로 전송 -> GM은 CM으로 전송 ->여기서부터는 완성
void URAIChatUI::AskSuspect(FText Text)
{
	RAIPlayerController->AskSuspect(Text);
	//TODO: UI를 소유하는 PC의 PC::AskSuspect() {GM::AskSuspect에 접근}
}