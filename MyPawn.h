// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"


#include "MyPawn.generated.h"



//헤더에서 전방선언->CPP에서 헤더 포함해야 링크됨
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent; //스프링암(삼각대)관련 전방선언
class UCameraComponent; //카메라 관련 전방 선언 
class UInputAction;
class UInputMappingContext;


//Enhanced Input에서 액션값을 받을 때 사용하는 구조체 FInputActionValue
struct FInputActionValue;

class UInputComponent;


UCLASS()
class CHAPTER2_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	//입력처리함수
	UFUNCTION() void MyMove(const FInputActionValue& value);
	UFUNCTION() void MyStartJump(const FInputActionValue& value);
	UFUNCTION() void MyStopJump(const FInputActionValue& value);
	UFUNCTION() void MyStartSprint(const FInputActionValue& value);
	UFUNCTION() void MyStopSprint(const FInputActionValue& value);
	UFUNCTION() void MyLook(const FInputActionValue& value);





protected:

	//Pawn이라서 CapsuleComponent만들어줘야함
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCapsuleComp")
	UCapsuleComponent* MyCapsuleComp;


	//Pawn이라서 SkeletalMeshComp만들어줘야함
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MySkeletalMeshComp")
	USkeletalMeshComponent* MySkeletalMeshComp;


	//BlueprintReadOnly 여도 블루프린트에서 속성은 변경가능하다(포인터는 변경불가인데)
	//그 포인터가 가르키는 속성은 변경 가능한것! 
	//스프링 암 삼각대 컴포넌트 멤버변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite/*리드온리여도 내부에선 조절가능?*/, Category = "MyCamera")
	USpringArmComponent* MySpringArmComp;

	//카메라 컴포넌트 멤버변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCamera")
	UCameraComponent* MyCameraComp;

	//화면돌리는 민감도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCamera")
	float LookSensitivity = 1.0f;



protected:
	//이동 속도 관련 properties

	//기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyMovement", meta = (DisplayPriority = 1))
	float NormalSpeed;

	// 기본 속도 대비 몇배로 달릴지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyMovement", meta = (DisplayPriority = 2))
	float SprintSpeedMultiplier;

	// 실제 스프린트 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyMovement", meta = (DisplayPriority = 3))
	float SprintSpeed;

};
