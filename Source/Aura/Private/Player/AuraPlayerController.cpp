// Copyright LeeWongeun


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/AuraEnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastHighlightActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/*
	* 1. 하이라이트 없었고 Hit 있는 경우 -> Hit Actor를 하이라이트
	* 2. 하이라이트 없었고 Hit 없는 경우 -> 아무 것도 안함
	* 3. 하이라이트 있었고 Hit 없는 경우 -> 하이라이트 액터 끄기
	* 4. 하이라이트 있었고 Hit 있는 경우, 하이라이트 액터 != Hit액터 -> 하이라이트 액터 끄고 Hit Actor를 하이라이트
	* 5. 하이라이트 있었고 Hit 있는 경우, 하이라이트 액터 == Hit액터 -> 아무것도 안함
	 */

	if (LastHighlightActor == nullptr)
	{
		//1번 case
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		//3번 case
		if (ThisActor == nullptr)
		{
			LastHighlightActor->UnHighlightActor();
		}
		else
		{
			//5번 case
			if (LastHighlightActor != ThisActor)
			{
				LastHighlightActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//인풋이 제대로 초기화되지 않으면 그냥 assert
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//cast 실패시 crash
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
