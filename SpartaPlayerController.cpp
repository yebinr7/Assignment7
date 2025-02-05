// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
ASpartaPlayerController::ASpartaPlayerController()
	:/*이니셜라이져에서 nullptr로 만들면서 초기화(컴파일타임)*/
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//현재 PlayerController에 연결된 Local Player 객체를 가져온다.
	if (ULocalPlayer* localPlayer = GetLocalPlayer())//"선언 + 할당 + nullptr 체크"를 동시에 수행
	{
		/*
		- **`GetLocalPlayer()`**
			 - 현재 PlayerController가 관리하는 Local Player를 반환합니다.
		  **`GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()`**
			 - 해당 Local Player에 부착된 Enhanced Input Subsystem을 반환합니다.
			 - 이를 통해 `AddMappingContext`나 `RemoveMappingContext` 등을 호출하여 입력 매핑을 동적으로 제어할 수 있습니다.
		  **`AddMappingContext()`**
			- 주어진 IMC를 Subsystem에 추가하여 입력 매핑을 활성화합니다.
			- `SpartaInputMappingContext`: 활성화할 IMC.
			- `0`: 우선순위. 낮을수록 높은 우선순위를 가집니다.
			- 이 함수를 여러 번 호출해 **여러 IMC**를 활성화할 수도 있습니다.
			- 우선순위를 달리 부여해, 특정 IMC가 다른 IMC보다 우선순위가 높도록 설정할 수도 있습니다.
		이 로직이 실행되면, 
		**IMC**에 정의된 모든 IA와 키 매핑이 이 PlayerController에 적용됩니다. 
		빌드 후 언리얼 에디터를 실행해 보세요.
		*/


		//Local Player에서 EnhancedInputLocal PlayerSubSystem을 획득
		if (UEnhancedInputLocalPlayerSubsystem* subSystem =
			localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				//SubSystem을 통해서 우리가 할당한 IMC를 활성화함
				//우선순위는 0이 가장 높은게 우선순위
				subSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
