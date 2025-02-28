// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/RAIHttpManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
//#include "JsonUtilities.h"
/*
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
*/

ARAIHttpManager::ARAIHttpManager()
{
	// Secrets.ini 경로 지정
	FString SecretsPath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
	GConfig->LoadFile(SecretsPath);

	//APIKey_OpenAI
	if (GConfig->GetString(TEXT("OpenAI"), TEXT("OpenAI_API_KEY"), APIKey_OpenAI, SecretsPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("APIKey_OpenAI Loaded: %s"), *APIKey_OpenAI);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load OpenAI_API_KEY from Secrets"));
	}

	//APIKey_NLP
	if (GConfig->GetString(TEXT("HuggingFace"), TEXT("HF_API_KEY"), APIKey_NLP, SecretsPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("APIKey_NLP Loaded: %s"), *APIKey_NLP);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load APIKey_NLP from Secrets"));
	}
}

void ARAIHttpManager::BeginPlay()
{
	Super::BeginPlay();

	// OpenAI Request와 Receive 테스트 코드
	/*
	SendRequestToOpenAI(FString::Printf(TEXT("좋은 아침이라고 해줘.")));
	SendRequestToOpenAI(FString::Printf(TEXT("내가 아까전에 뭘 말해달라고 했지?")));
	*/
	//SendRequestToNLP(FString::Printf(TEXT("(입술을 떨며, 손에 땀을 흘리고 얼굴이 일그러짐) 그게... 그게 무슨 뜻이에요 ? 제가 그 커튼을 만졌다고요 ? 그럴 리 없어요!제가 그런 짓을 할 이유가 없잖아요.그런 자국을 남기게 될 이유가... 없다고요!(심장이 빠르게 뛰고, 숨을 고르지 못함) 저는 정말로 그런 일을 하지 않았어요! 제발 믿어주세요!")));

}

//문장 전송
void ARAIHttpManager::SendRequestToOpenAI(const FString& InputText)
{
	// HTTP 요청 생성    
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnOpenAIResponse); //단일 delegate
	Request->SetURL(URL_OpenAI);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey_OpenAI));


	// 요청 본문 설정
	TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject);
	RequestBody->SetStringField("model", "gpt-4o-mini"); // 사용 모델 설정. gpt-4o-mini 선택
	RequestBody->SetNumberField("max_tokens", 100); // 응답 길이 설정
	RequestBody->SetNumberField("temperature", 0.7); // 창의성 설정

	// 'messages' 배열 생성
	TArray<TSharedPtr<FJsonValue>> MessagesArray;
	TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
	UserMessage->SetStringField("role", "user");
	UserMessage->SetStringField("content", InputText);
	MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));

	RequestBody->SetArrayField("messages", MessagesArray);

	// JSON 직렬화
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);
	Request->SetContentAsString(OutputString);

	if (Request->ProcessRequest()) // 요청 보내기
	{
		UE_LOG(LogTemp, Log, TEXT("OpenAI Request Send Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("OpenAI Request Send Failed"));
	}

}

//문장 수신
void ARAIHttpManager::OnOpenAIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("OpenAI Request failed"));
		return;
	}
	FString ResponseString = Response->GetContentAsString();
	// JSON 문자열을 JSON 객체로 파싱
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> ChoicesArray;
		ChoicesArray = JsonObject->GetArrayField(TEXT("choices"));
		TSharedPtr<FJsonObject> ChoiceObject = ChoicesArray[0]->AsObject();
		TSharedPtr<FJsonObject> MessageObject = ChoiceObject->GetObjectField(TEXT("message"));
		FString ContentString = MessageObject->GetStringField(TEXT("content"));

		SendRequestToNLP(ContentString);
		ResponseDelegate_OpenAI.Broadcast(ContentString);
	}
}

void ARAIHttpManager::SendRequestToNLP(const FString& InputText)
{
	// HTTP 요청 생성
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnNLPResponse);
	Request->SetURL(URL_NLP);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey_NLP));

	// 요청 본문 설정
	TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject()); //RequestBody = JsonObject
	//messages 배열 생성
	RequestBody->SetStringField(TEXT("inputs"), InputText);

	// JSON 직렬화
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);
	Request->SetContentAsString(OutputString);

	// 요청 실행
	if (Request->ProcessRequest()) // 요청 보내기
	{
		UE_LOG(LogTemp, Log, TEXT("NLP Request Send Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("NLP Request Send Failed"));
	}
}

void ARAIHttpManager::OnNLPResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("NLP Request Failed"));
		return;
	}

	ensure(ResponseDelegate_NLP.IsBound());
	ResponseDelegate_NLP.Broadcast(Response->GetContentAsString());
}

