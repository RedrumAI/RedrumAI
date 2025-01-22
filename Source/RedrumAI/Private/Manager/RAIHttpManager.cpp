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
    // Secrets.ini ��� ����
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

    // OpenAI Request�� Receive �׽�Ʈ �ڵ�
    SendRequestToOpenAI(FString::Printf(TEXT("Say just Hi")));
}

//���� ����
    void ARAIHttpManager::SendRequestToOpenAI(const FString& Prompt)
    {
        // OpenAI API URL
        FString Url = "https://api.openai.com/v1/chat/completions"; // chat model�� ���

        // HTTP ��û ����    
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
        Request->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnResponseReceived);
        Request->SetURL(Url);
        Request->SetVerb("POST");
        Request->SetHeader("Content-Type", "application/json");
        Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *OpenAI_API_Key));
    
     
        // ��û ���� ����
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
        JsonObject->SetStringField("model", "gpt-4o-mini"); // ��� �� ����. gpt-4o-mini ����
        JsonObject->SetNumberField("max_tokens", 100); // ���� ���� ����
        JsonObject->SetNumberField("temperature", 0.7); // â�Ǽ� ����
        // 'messages' �迭 ����
        TArray<TSharedPtr<FJsonValue>> MessagesArray;
        TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
        UserMessage->SetStringField("role", "user");
        UserMessage->SetStringField("content", Prompt);
        MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));
        JsonObject->SetArrayField("messages", MessagesArray);
    
        // JSON ����ȭ
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
        Request->SetContentAsString(OutputString);


        if (Request->ProcessRequest()) // ��û ������
        {
            UE_LOG(LogTemp, Log, TEXT("Send Success"));
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Send Failed"));
        }
    
    }

//���� ����
void ARAIHttpManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseString = Response->GetContentAsString();

        // JSON ���ڿ��� �α׷� ���
        UE_LOG(LogTemp, Log, TEXT("Full Response: %s"), *ResponseString);

        // JSON ���ڿ��� JSON ��ü�� �Ľ�
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // JSON ��ü�� �ٽ� ���ڿ��� ����ȭ�Ͽ� ��ü ���� ���
            FString JsonPrettyString;
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonPrettyString);
            FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

            // ����ȭ�� JSON ������ �α׷� ���
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

        // ���� Response�� ��� ������� ����ʹ�.


        /*
        // ������ȭ
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // 'choices' �ʵ忡�� ù ��° ��Ҹ� �����ɴϴ�.
            TArray<TSharedPtr<FJsonValue>> ChoicesArray = JsonObject->GetArrayField("choices");
            if (ChoicesArray.Num() > 0 && ChoicesArray[0].IsValid())
            {
                // ù ��° ��Ҹ� FJsonObject�� ��ȯ
                TSharedPtr<FJsonObject> ChoiceObject = ChoicesArray[0]->AsObject();
                if (ChoiceObject.IsValid())
                {
                    // 'text' �ʵ忡 �����Ͽ� ���ڿ� ��������
                    FString Answer = ChoiceObject->GetStringField("text");
                    UE_LOG(LogTemp, Log, TEXT("OpenAI Response: %s"), *Answer);
                    // ������ ���� ���� ǥ���ϰų� �ٸ� �۾��� ���
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

