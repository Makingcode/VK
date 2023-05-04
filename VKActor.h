// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VKActor.generated.h"

//키보드 스테이트
UENUM(BlueprintType)
enum class EVirtualKeyBoardState : uint8
{
	EN_Lower UMETA(DisplayName = "Eng Lower"),
	EN_Upper UMETA(DisplayName = "Eng Upper"),
	KR_Lower UMETA(DisplayName = "Kr Lower"),
	KR_Upper UMETA(DisplayName = "Kr Upper")

};

UCLASS()
class AVKActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVKActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void ReleasedShift();
	void PressedShift();

	UFUNCTION(BlueprintCallable, Category = "LIB_VK")
	void SendKeyInput(EVirtualKeyBoardState KeyLangState, FString LastString, int inputkeycode);

	UFUNCTION(BlueprintCallable, Category = "LIB_VK")
	void SetCurrentInputMode(bool IsKR);

	UFUNCTION(BlueprintCallable, Category="LIB_VK")
	FString SubstringLastWord(FString InString);


	//str 전체문자열, buffer, 반환받을 버퍼, nSize 버퍼 사이즈
	int BreakStr(TCHAR* str, TCHAR* buffer, UINT nSize);

	//백스페이스 입력처리
	UFUNCTION(BlueprintCallable, Category = "LIB_VK")
	void BackSpaceFunc();

public:
	DWORD KeyCode = 0;
	

};
