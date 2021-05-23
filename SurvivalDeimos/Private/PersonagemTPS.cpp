// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonagemTPS.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Arma.h"
#include "Engine/EngineTypes.h"
#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"



// Sets default values
APersonagemTPS::APersonagemTPS()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmCamera"));
	SpringArmCamera->TargetArmLength = 200.f;
	SpringArmCamera->bUsePawnControlRotation = true;
	SpringArmCamera->AddRelativeLocation(FVector(0.f, 40.f, 50.f));
	SpringArmCamera->bEnableCameraLag = true;
	SpringArmCamera->CameraLagMaxDistance = 40.f;
	SpringArmCamera->SetupAttachment(RootComponent);

	CameraPersonagem = CreateDefaultSubobject<UCameraComponent>(FName("CameraPersonagem"));
	CameraPersonagem->SetupAttachment(SpringArmCamera);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = .425f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->CrouchedHalfHeight = 70.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APersonagemTPS::Atirar()
{
	ArmaPlayer->Atirar();
}


// Called when the game starts or when spawned
void APersonagemTPS::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	/* aqui tinhamos uma variavel declarada dentro da fun��o BeginPlay. Ou seja,
	disponivel apenas neste escopo de codigo
	mas n�s queremos que esta vari�vel este disponivel em toda a classe*/
	ArmaPlayer = GetWorld()->SpawnActor<AArma>(BP_ArmaTipoRifle,
		FTransform(), Parametros);
	ArmaPlayer->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetIncludingScale,
		FName("SocketDaArma"));
}

void APersonagemTPS::MoverParaFrente(float Valor)
{
	AddMovementInput(GetActorForwardVector() * Valor);
}

void APersonagemTPS::MoverDireita(float Valor)
{
	AddMovementInput(GetActorRightVector() * Valor);
}

void APersonagemTPS::Pular()
{
	BEstaPulando = true;
}

void APersonagemTPS::PararPulo()
{
	BEstaPulando = false;
}

void APersonagemTPS::Agachar()
{
	Crouch();
}


void APersonagemTPS::Levantar()
{
	UnCrouch();
}

// Called every frame
void APersonagemTPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APersonagemTPS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoverParaFrente", this, &APersonagemTPS::MoverParaFrente);
	PlayerInputComponent->BindAxis("MoverDireita", this, &APersonagemTPS::MoverDireita);
	PlayerInputComponent->BindAxis("OlharCimaBaixo", this, &APersonagemTPS::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("OlharDireitaEsquerda", this, &APersonagemTPS::AddControllerYawInput);

	PlayerInputComponent->BindAction("Agachar", EInputEvent::IE_Pressed, this, &APersonagemTPS::Agachar);
	PlayerInputComponent->BindAction("Agachar", EInputEvent::IE_Released, this, &APersonagemTPS::Levantar);

	PlayerInputComponent->BindAction("Pular", EInputEvent::IE_Pressed, this, &APersonagemTPS::Pular);
	PlayerInputComponent->BindAction("Pular", EInputEvent::IE_Released, this, &APersonagemTPS::PararPulo);

	PlayerInputComponent->BindAction("Atirar", EInputEvent::IE_Pressed, this, &APersonagemTPS::Atirar);

}

void APersonagemTPS::SetHealth(float Dano)
{
	if (Health > 0.0f)
	{
		Health -= Dano;
	}
	else if (Health <= 0.0f)
	{
		bEstaMorto = true;
	}

}

float APersonagemTPS::GetHealth()
{
	return Health;
}

bool APersonagemTPS::IsEstaMorto()
{
	return bEstaMorto;
}

