// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIHttpManager.h"
#include "Http.h"

// Sets default values
ARAIHttpManager::ARAIHttpManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Secrets.ini ��� ����
	FString SecretsPath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
	GConfig->LoadFile(SecretsPath);

	if (GConfig->GetString(TEXT("HuggingFace"), TEXT("HF_API_KEY"), HF_API_KEY, SecretsPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("HF_API_KEY Loaded: %s"), *HF_API_KEY);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load HF_API_KEY from Secrets"));
	}

    APIEndpoint = TEXT("https://api-inference.huggingface.co/models/distilbert-base-uncased-finetuned-sst-2-english");
        
}

// Called when the game starts or when spawned
void ARAIHttpManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARAIHttpManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARAIHttpManager::SendRequest(const FString& InputText)
{
    // HTTP ��û ����
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    // URL ����
    HttpRequest->SetURL(APIEndpoint);

    // �޼��� ���� (POST)
    HttpRequest->SetVerb(TEXT("POST"));

    // ��� ����
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *HF_API_KEY));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // ��û �ٵ� ����
    TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject());
    RequestBody->SetStringField(TEXT("inputs"), InputText);

    FString RequestBodyString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyString);
    FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(RequestBodyString);

    // ���� �ڵ鷯 ���ε�
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnResponseReceived);

    // ��û ����
    HttpRequest->ProcessRequest();
}

void ARAIHttpManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}
