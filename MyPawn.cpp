// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
//카메라, 스프링암 실제 구현 ㄱㄱ
#include "Components/CapsuleComponent.h" // 루트 컴포넌트가 캡슐이라면 필요
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"// Sets default values
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"//

//
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

AMyPawn::AMyPawn()
	:NormalSpeed(100.0f),
	SprintSpeedMultiplier(2.0f),
	SprintSpeed(NormalSpeed* SprintSpeedMultiplier)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//틱함수 일단 꺼두기
	
	//1. 충돌 캡슐 생성 & 루트에 세팅
	MyCapsuleComp =
		CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	//MyCapsuleComp->SetupAttachment(RootComponent);
	RootComponent = MyCapsuleComp;

	//2. 스켈레탈 메시 comp 추가
	//생성자에서 캐릭터는 기본으로 설정되어 있지만 pawn은 내가 만들어줘야함 
	MySkeletalMeshComp =
		CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MySkeletalMeshComp->SetupAttachment(MyCapsuleComp);


	//3. 스프링암 생성(삼각대)
	MySpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// 스프링 암을 루트 컴포넌트에(캡슐컴포넌트)에 부착
	MySpringArmComp->SetupAttachment(MyCapsuleComp); //캐릭터라서 기본적으로 루트 컴포넌트가 있는건가?;
	//캐릭터와 카메라 사이의 거리 기본값 300으로 설정
	MySpringArmComp->TargetArmLength = 300.0f;
	
	
	
	//컨트롤러 회전에 따라 스프링암도 회전하도록 설정
	//SpringArm이 독립적으로 회전할 수 있도록 아래와 같이 설정을 변경!!!

	MySpringArmComp->bUsePawnControlRotation = false; // Pawn의 회전을 따라가지 않도록 설정
	MySpringArmComp->bInheritPitch = false; //// Pitch 회전 독립적으로 설정
	MySpringArmComp->bEnableCameraLag = true;  // 부드러운 카메라 이동
	MySpringArmComp->CameraLagSpeed = 5.0f;



	//4. 카메라 컴포넌트 생성
	MyCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//스프링 암의 소켓 위치에 카메라를 부착 마지막에 붙히는거라 SocketName?
	MyCameraComp->SetupAttachment(MySpringArmComp, USpringArmComponent::SocketName);
	//카메라는 스프링암의 회전을 따르므로 PawnControlRotation은 꺼둠
	MyCameraComp->bUsePawnControlRotation = false;

	












	// 루트 충돌 컴포넌트가 캡슐이면 물리 비활성화
	//if (UCapsuleComponent* CapsuleComp = Cast<UCapsuleComponent>(RootComponent))//위에서 당연히캡슐컴포넌트
	//{
	//	CapsuleComp->SetSimulatePhysics(false);
	//}

	//루트 충돌 컴포넌트 물리 비활성화
	MyCapsuleComp->SetSimulatePhysics(false);
	// SkeletalMeshComponent의 물리 비활성화
	MySkeletalMeshComp->SetSimulatePhysics(false);

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->Enhanced Input Component로 다운캐스팅
	UEnhancedInputComponent* enhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInput)//캐스팅됐다면..
	{
		if (AMyPlayerController* playerController =
			Cast<AMyPlayerController>(GetController()))//가져온컨트롤러 다운캐스팅됐다면
		{

			//내가 만든 Move함수(IA_Move 를 지정한 변수)
			if (playerController->MoveAction)
			{
				enhancedInput->BindAction(
					playerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::MyMove//내가 구현한 Move바인딩
					);
			}



			//내가 만든 Look
			//Look
			if (playerController->LookAction)
			{
				//IA_Look 액션 마우스가 움직일때 SpartaPlayerController의 Look호출
				enhancedInput->BindAction(
					playerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::MyLook // Look
				);
			}


		}


	}




}



void AMyPawn::MyMove(const FInputActionValue& value)
{
	//pawn의 무브 구현
	//value 가 X,Y 로 들어옴 

	if (!Controller) return;
	const FVector2D movementVector = value.Get<FVector2D>();

	FVector moveDirection = FVector::ZeroVector;
	//IA에서 X를 Forward로
	if (!FMath::IsNearlyZero(movementVector.X))
	{
		moveDirection += GetActorForwardVector() * movementVector.X;
	}
	if (!FMath::IsNearlyZero(movementVector.Y))
	{
		moveDirection += GetActorRightVector() * movementVector.Y;
	}

	//최종이동(속도 * 시간)
	if (!moveDirection.IsNearlyZero())
	{
		AddActorLocalOffset(moveDirection * NormalSpeed * GetWorld()->GetTimeSeconds(), true/*충돌감지여부*/);
	}


	//IA 에서 순서 변경해놨음!!! 그래서 X가 forward가 맞음 
	/*FVector xForward = FVector::ZeroVector;
	FVector yRight = FVector::ZeroVector;*/


	//if (!FMath::IsNearlyZero(movementVector.X))
	//{
	//	//AddMovementInput는 ACharacter에서 제공하는 기능.. Pawn은 내가 직접구현
	//	 xForward = //시간은 월드에서 델타타임 가져와서 거속시 
	//		GetActorForwardVector() * movementVector.X * NormalSpeed * GetWorld()->GetDeltaSeconds();
	//}
	//if (!FMath::IsNearlyZero(movementVector.Y))
	//{
	//	 yRight =
	//		GetActorRightVector() * movementVector.Y * NormalSpeed * GetWorld()->GetDeltaSeconds();
	//}
	////X요소 + Y요소 -> 2D벡터
	//AddActorLocalOffset(xForward + yRight, true);
}

void AMyPawn::MyStartJump(const FInputActionValue& value)
{
}

void AMyPawn::MyStopJump(const FInputActionValue& value)
{
}

void AMyPawn::MyStartSprint(const FInputActionValue& value)
{
}

void AMyPawn::MyStopSprint(const FInputActionValue& value)
{
}

void AMyPawn::MyLook(const FInputActionValue& value)
{

	FVector2D lookInput = value.Get<FVector2D>();

	// 마우스 X축 이동 → Yaw 회전 (Z축 회전)
	if (lookInput.X != 0.0f)
	{
		FRotator yawRotation = FRotator(0.f, lookInput.X * LookSensitivity, 0.f);
		AddActorLocalRotation(yawRotation);
	}

	// 마우스 Y축 이동 → Pitch 회전 (Y축 회전)
	if (lookInput.Y != 0.0f)
	{
		// 현재 SpringArm의 상대적인 회전을 가져옴
		FRotator newPitchRotation = MySpringArmComp->GetRelativeRotation();
		newPitchRotation.Pitch = FMath::Clamp(newPitchRotation.Pitch + lookInput.Y * LookSensitivity, -80.f, 80.f);

		// 새로운 Pitch 값 적용
		MySpringArmComp->SetRelativeRotation(newPitchRotation);
	}

	//FVector2D lookInput = value.Get<FVector2D>();
	//// 마우스의 X, Y 좌표 
	//// 
	//// 마우스의 X축 -> 언리얼에선 Z축회전해야함(yaw회전)
	//FRotator newYawRotation = GetActorRotation();
	//newYawRotation.Yaw += lookInput.X * LookSensitivity;
	//SetActorRotation(newYawRotation);

	////마우스의 화면 Y축 -> 언리얼에선 Y축회전 (pitch회전) 순서반대로 IA설정해뒀음
	//FRotator newPitchRotation = MySpringArmComp->GetRelativeRotation();
	//newPitchRotation.Pitch = FMath::Clamp(newPitchRotation.Pitch + lookInput.Y * LookSensitivity,
	//	-80.f, 80.f/*Clamp통해서 Pitch값이 -80~80 범위로 제한!->360도 카메라 회전방지*/);

	////새로운 pitch로 업데이트 
	//MySpringArmComp->SetRelativeRotation(newPitchRotation);
}


