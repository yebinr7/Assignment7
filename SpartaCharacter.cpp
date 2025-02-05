// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"

#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"

//카메라, 스프링암 실제 구현 ㄱㄱ
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//스프린트관련 헤더
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASpartaCharacter::ASpartaCharacter()
	:NormalSpeed(600.0f),
	 SprintSpeedMultiplier(3.0f),
	SprintSpeed(NormalSpeed*SprintSpeedMultiplier)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//틱함수 일단 꺼두기 

	//1. 스프링암 생성(삼각대)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	// 스프링 암을 루트 컴포넌트에(캡슐컴포넌트)에 부착
	SpringArmComp->SetupAttachment(RootComponent); //캐릭터라서 기본적으로 루트 컴포넌트가 있는건가?;

	//캐릭터와 카메라 사이의 거리 기본값 300으로 설정
	SpringArmComp->TargetArmLength = 300.0f;
	
	//컨트롤러 회전에 따라 스프링암도 회전하도록 설정
	SpringArmComp->bUsePawnControlRotation = true;

	//2. 카메라 컴포넌트 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//스프링 암의 소켓 위치에 카메라를 부착 마지막에 붙히는거라 SocketName?
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//카메라는 스프링암의 회전을 따르므로 PawnControlRotation은 꺼둠
	CameraComp->bUsePawnControlRotation = false;




	//내가 설정한 기본 걷기속도 캐릭터무브먼트에설정
	//NormalSpeed = 600.0f;
	//SprintSpeedMultiplier = 1.5f;
	////스프린트 설정
	//SprintSpeed = NormalSpeed * SprintSpeedMultiplier;


	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

// Called when the game starts or when spawned
void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// UInputComponent -> Enhanced InputComponent로 캐스팅(다운캐스팅?) 
	if (UEnhancedInputComponent* enhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//IA를 가져오기 위해 소유중인 Controller 를 ASpartaPlayerController로 캐스팅
		if (ASpartaPlayerController* playerController =
			Cast<ASpartaPlayerController>(GetController()))
		{

			//내가 정의한 SpartaPlayerController로 일단 다운캐스팅하고
			//EnhancedInputComponent를 얻어온뒤, 그걸 잘 섞어서 
			//내가 정의한 함수 실행하면->실질적으론 언리얼에서 만든 함수 부를수있게 





			//Move
			if (playerController->MoveAction) //SpartaController클래스가 가지고있는 MoveAction메서드
			{
				//IA_Move 액션 키를 "키를 누르고 있는 동안" Move()
				//실제 움직이도록
				enhancedInput->BindAction(					//
					playerController->MoveAction,			//첫 번째 인자: 어떤 UInputAction과 연결할지. (예: MoveAction)
					ETriggerEvent::Triggered,				//두 번째 인자: 액션이 발생하는 트리거 이벤트 (Triggered, Ongoing, Completed 등).
					this,									//세 번째/네 번째 인자: 액션 발생 시 실행할 객체(this)와 함수 포인터.
					&ASpartaCharacter::Move //Move			//
				);
			}


			//Jump
			if(playerController->JumpAction)
			{
				//점프 시작
				enhancedInput->BindAction(
					playerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Jump //  Jump
				);

				//점프 끝
				enhancedInput->BindAction(
					playerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump //  Jump
				);
			}



			//Look
			if (playerController->LookAction)
			{
				//IA_Look 액션 마우스가 움직일때 SpartaPlayerController의 Look호출
				enhancedInput->BindAction(
					playerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look // Jump
				);
			}

			//Sprint Start & Stop
			if (playerController->SprintAction)
			{
				//StartSprint()호출됨 내가만든거
				enhancedInput->BindAction(
					playerController->SprintAction,//내가만든거
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint // S
				);


				//StartSprint()호출됨 내가만든거
				enhancedInput->BindAction(
					playerController->SprintAction,//내가만든거
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint // S
				);
			}
		}
	}

}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	//실제 무브 구현
	//컨트롤러가 있어야 방향 계산이 가능
	if (!Controller) return;//없다면 계산 못하니깐 반환

	// Value 구조체는 WASD입력값을 X,Y축 2차평면 좌표로 가지고 있음
	// 앞W V{1,0} 뒤S V{-1,0} 왼A V{0,-1}, 오D V{0,1} === 왼손좌표계 생각! X가 캐릭터 보는 방향 
	

	//2D 벡터 입력 값(방향 + 크기)을 저장하는 변수입니다. 방향도 얻을수 있고 크기도 얻을 수 있음(기능있는 구조체)
	const FVector2D moveInput = value.Get<FVector2D>(); 

	if (!FMath::IsNearlyZero(moveInput.X))//들어온 float X값이 0과 거의 가깝지 않다면->이동한다면..
	{
		//AddMovementInput()는 키 입력 때마다 “어느 방향으로 이동”할지 전달하는 함수

		//캐릭터가 바라보는 방향 X축이동
		AddMovementInput(GetActorForwardVector()/*방향X 1값?*/, moveInput.X/*X의 속도크기 X좌표값*/); //폰함수? X로 가는벡터
	}

	if (!FMath::IsNearlyZero(moveInput.Y))//들어온 float Y값이 0과 거의 가깝지 않다면->이동한다면..
	{
		//캐릭터 오른쪽 이동 == Y축이동
		AddMovementInput(GetActorRightVector(), moveInput.Y); //폰함수? Y로 가는벡터
	}

	/*
	* 내부적으로 CharacterMovementComponent가 이 요청을 받아 속도를 계산하고, 실제 이동을 구현합니다.
	*/
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	//Jump 함수는 Character가 기본으로 제공
	if (value.Get<bool>())
		Jump();
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
		StopJumping();
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookInput = value.Get<FVector2D>();
	// 마우스의 X, Y 좌표 
	
	// 마우스의 X축 -> 언리얼에선 Z축회전해야함(yaw회전)
	AddControllerYawInput(lookInput.X/*벡터의x값넣기*/);

	// 마우스의 Y축 -> 언리얼에선 Y축을 기준으로 회전(pitch회전) , X축이 바라보는 방향이니깐 
	AddControllerPitchInput(lookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	//shift 누르면 이 함수가 호출된다고 가정
	//스프린트 속도를 적용
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; //생성자에서 걷기 스피드로 설정되어있었는데 -> 시프트누르면 SprintSpeed로 설정
	}

}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	//shift 떼지면 이 함수가 호출된다고 가정
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; //시프트 떼진상태니깐 노말스피드로 바꾸기
	}
}

