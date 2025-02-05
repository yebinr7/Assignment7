// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h" //Enhanced Input System의 Local Player SubSystem 사용하기 위해
AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//현재 PlayerController에 연결된 Locla Player 객체를 가져온다.
	if (ULocalPlayer* localPlayer = GetLocalPlayer())//가져왔다면...
	{
		//Local Player객체 멤버함수 GetSubSystem 을 가져옴
		if (UEnhancedInputLocalPlayerSubsystem* subSystem =
			localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())//가져왔다면..
		{
			if (InputMappingContext)//InputMappingContext가 존재한다면..
				//(처음엔 생성자이니셜nullptr이지만 블루프린트에서 설정해줬으므로?; IMC존재)
			{
				//SubSystem을 통해 우리가 할당한 IMC(InputMappingContext)를 활성화시키자
				subSystem->AddMappingContext(InputMappingContext, 0/*가장높은우선순위*/);
			}
		}
	}
}
