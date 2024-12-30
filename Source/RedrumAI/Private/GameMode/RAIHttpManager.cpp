// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIHttpManager.h"
#include "Http.h"

// Sets default values
ARAIHttpManager::ARAIHttpManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Secrets.ini 경로 지정
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
    // HTTP 요청 생성
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    // URL 설정
    HttpRequest->SetURL(APIEndpoint);

    // 메서드 설정 (POST)
    HttpRequest->SetVerb(TEXT("POST"));

    // 헤더 설정
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *HF_API_KEY));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // 요청 바디 설정
    TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject());
    RequestBody->SetStringField(TEXT("inputs"), InputText);

    FString RequestBodyString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyString);
    FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(RequestBodyString);

    // 응답 핸들러 바인딩
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARAIHttpManager::OnResponseReceived);

    // 요청 실행
    HttpRequest->ProcessRequest();
}

void ARAIHttpManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

