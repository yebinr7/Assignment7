// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

//헤더에서 전방선언->CPP에서 헤더 포함해야 링크됨
class USpringArmComponent; //스프링암(삼각대)관련 전방선언
class UCameraComponent; //카메라 관련 전방 선언 


//Enhanced Input에서 액션값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class CHAPTER2_API ASpartaCharacter : public ACharacter
{
GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpartaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FInputActionValue 구조체라 const & 로 받아와야 안전하고 복사안하고 레퍼런스로 받아옴
	//블루프린트 접근성 설정안해도 기본적으로 메타데이터가 생성되기에, 리플렉션 시스템과 연동시켜야함
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

protected:

	//BlueprintReadOnly 여도 블루프린트에서 속성은 변경가능하다(포인터는 변경불가인데)
	//그 포인터가 가르키는 속성은 변경 가능한것! 
	//스프링 암 삼각대 컴포넌트 멤버변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly/*리드온리여도 내부에선 조절가능?*/, Category = "Camera");
	USpringArmComponent* SpringArmComp;

	//카메라 컴포넌트 멤버변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;



protected:
	//이동 속도 관련 properties

	//기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAA|MyMovement", meta = (DisplayPriority = 1))
	float NormalSpeed;

	// 기본 속도 대비 몇배로 달릴지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAA|MyMovement", meta = (DisplayPriority = 2))
	float SprintSpeedMultiplier;

	// 실제 스프린트 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAA|MyMovement", meta = (DisplayPriority = 3))
	float SprintSpeed; 

	//float NormalSpeed;
	//float SprintSpeedMultiplier;
	//float SprintSpeed;


};
