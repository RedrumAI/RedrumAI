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

    // OpenAI Request�� Receive �׽�Ʈ �ڵ�
    //SendRequestToOpenAI(FString::Printf(TEXT("Say just Hi")));

    SendRequestToNLP(FString::Printf(TEXT("Today is Monday. I Have to Study. Im so Sad.")));
}

//���� ����
void ARAIHttpManager::SendRequestToOpenAI(const FString& InputText)
{
    // HTTP ��û ����    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnOpenAIResponse); //���� delegate
    Request->SetURL(URL_OpenAI);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey_OpenAI));
    
     
    // ��û ���� ����
    TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject);
    RequestBody->SetStringField("model", "gpt-4o-mini"); // ��� �� ����. gpt-4o-mini ����
    RequestBody->SetNumberField("max_tokens", 100); // ���� ���� ����
    RequestBody->SetNumberField("temperature", 0.7); // â�Ǽ� ����
    // 'messages' �迭 ����
    TArray<TSharedPtr<FJsonValue>> MessagesArray;
    TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
    UserMessage->SetStringField("role", "user");
    UserMessage->SetStringField("content", InputText);
    MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));
    RequestBody->SetArrayField("messages", MessagesArray);
    
    // JSON ����ȭ
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);
    Request->SetContentAsString(OutputString);


    if (Request->ProcessRequest()) // ��û ������
    {
        UE_LOG(LogTemp, Log, TEXT("OpenAI Request Send Success"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("OpenAI Request Send Failed"));
    }
    
}

//���� ����
void ARAIHttpManager::OnOpenAIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("OpenAI Request failed"));
        return;
    }

    // ���� �Ľ�
    FString ResponseString = Response->GetContentAsString();

    // JSON ���ڿ��� �α׷� ���
    UE_LOG(LogTemp, Log, TEXT("OpenAI Full Response: %s"), *ResponseString);

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

void ARAIHttpManager::SendRequestToNLP(const FString& InputText)
{
    // HTTP ��û ����
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnNLPResponse);
    Request->SetURL(URL_NLP);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey_NLP));

    // ��û ���� ����
    TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject()); //RequestBody = JsonObject
    //messages �迭 ����
    RequestBody->SetStringField(TEXT("inputs"), InputText);

    // JSON ����ȭ
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);
    Request->SetContentAsString(OutputString);

    // ��û ����
    if (Request->ProcessRequest()) // ��û ������
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

    // ���� �Ľ�
    FString ResponseString = Response->GetContentAsString();

    // JSON ���ڿ��� �α׷� ���
    UE_LOG(LogTemp, Log, TEXT("NLP Full Response: %s"), *ResponseString);

    TArray<TSharedPtr<FJsonValue>> JsonResponse;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonResponse) && JsonResponse.Num() > 0)
    {
        TArray<TSharedPtr<FJsonValue>> InnerArray = JsonResponse[0]->AsArray();

        // TODO: Label�� Score�� �̾Ƴ��� �ڵ�


        //UE_LOG(LogTemp, Log, TEXT("Label: %s, Score: %f"), *Label, Score);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
    }
}

