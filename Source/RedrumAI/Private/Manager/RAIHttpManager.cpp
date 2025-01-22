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

    if (GConfig->GetString(TEXT("OpenAI"), TEXT("OpenAI_API_KEY"), OpenAI_API_Key, SecretsPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("OpenAI_API_KEY Loaded: %s"), *OpenAI_API_Key);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load OpenAI_API_KEY from Secrets"));
    }
}

void ARAIHttpManager::BeginPlay()
{
    Super::BeginPlay();

    // OpenAI Request와 Receive 테스트 코드
    SendRequestToOpenAI(FString::Printf(TEXT("Say just Hi")));
}

//문장 전송
    void ARAIHttpManager::SendRequestToOpenAI(const FString& Prompt)
    {
        // OpenAI API URL
        FString Url = "https://api.openai.com/v1/chat/completions"; // chat model과 통신

        // HTTP 요청 생성    
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
        Request->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnResponseReceived);
        Request->SetURL(Url);
        Request->SetVerb("POST");
        Request->SetHeader("Content-Type", "application/json");
        Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *OpenAI_API_Key));
    
     
        // 요청 본문 설정
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
        JsonObject->SetStringField("model", "gpt-4o-mini"); // 사용 모델 설정. gpt-4o-mini 선택
        JsonObject->SetNumberField("max_tokens", 100); // 응답 길이 설정
        JsonObject->SetNumberField("temperature", 0.7); // 창의성 설정
        // 'messages' 배열 생성
        TArray<TSharedPtr<FJsonValue>> MessagesArray;
        TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
        UserMessage->SetStringField("role", "user");
        UserMessage->SetStringField("content", Prompt);
        MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));
        JsonObject->SetArrayField("messages", MessagesArray);
    
        // JSON 직렬화
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
        Request->SetContentAsString(OutputString);


        if (Request->ProcessRequest()) // 요청 보내기
        {
            UE_LOG(LogTemp, Log, TEXT("Send Success"));
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Send Failed"));
        }
    
    }

//문장 수신
void ARAIHttpManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseString = Response->GetContentAsString();

        // JSON 문자열을 로그로 출력
        UE_LOG(LogTemp, Log, TEXT("Full Response: %s"), *ResponseString);

        // JSON 문자열을 JSON 객체로 파싱
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // JSON 객체를 다시 문자열로 직렬화하여 전체 내용 출력
            FString JsonPrettyString;
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonPrettyString);
            FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

            // 직렬화된 JSON 내용을 로그로 출력
            UE_LOG(LogTemp, Log, TEXT("Parsed JSON Content: %s"), *JsonPrettyString);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Request failed"));
    }
    /*
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseString = Response->GetContentAsString();
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

        // 받은 Response가 어떻게 생겼는지 보고싶다.


        /*
        // 역직렬화
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // 'choices' 필드에서 첫 번째 요소를 가져옵니다.
            TArray<TSharedPtr<FJsonValue>> ChoicesArray = JsonObject->GetArrayField("choices");
            if (ChoicesArray.Num() > 0 && ChoicesArray[0].IsValid())
            {
                // 첫 번째 요소를 FJsonObject로 변환
                TSharedPtr<FJsonObject> ChoiceObject = ChoicesArray[0]->AsObject();
                if (ChoiceObject.IsValid())
                {
                    // 'text' 필드에 접근하여 문자열 가져오기
                    FString Answer = ChoiceObject->GetStringField("text");
                    UE_LOG(LogTemp, Log, TEXT("OpenAI Response: %s"), *Answer);
                    // 응답을 게임 내에 표시하거나 다른 작업에 사용
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Request failed"));
    }
    */
}

