// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

class UInputMappingContext; //Input Mapping Context IMC관련전방선언
class UInputAction;  //InputAction IA관련 선언

/**
 * 
 */
UCLASS()
class CHAPTER2_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASpartaPlayerController();

	// 에디터에서 세팅할 IMC + 블루프린터 리플렉션 설정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	//IA_Move 를 지정할 변수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	//IA_Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	//IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	//IA_Sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	

	//Input Mapping Context: IMC 활성화하기위해
	virtual void BeginPlay() override;
};
