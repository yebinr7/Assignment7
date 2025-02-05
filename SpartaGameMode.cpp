// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"//게임모드에 PlayerController 사용하겠다 
ASpartaGameMode::ASpartaGameMode()
{
	/*
	ASpartaCharacter::StaticClass()는 
	런타임에 클래스 정보를 반환하는 함수입니다.
	이니셜라이져는 컴파일타임 이므로 불가능! 
	*/

	//✔ 즉, StaticClass()는 런타임에 UClass 메타데이터를 반환하는 함수입니다.->UClass객체 반환
	// 그 메타데이터를 등록하는느낌(인스턴스 생성하여 플레이어에게 할당)
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();//내가만든커스텀컨트롤러 등록
	
}
