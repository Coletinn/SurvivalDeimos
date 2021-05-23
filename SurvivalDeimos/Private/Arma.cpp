// Fill out your copyright notice in the Description page of Project Settings.


#include "Arma.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Animation/SkeletalMeshActor.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"
#include "PersonagemTPS.h"
#include "BotCharacter.h"

// Sets default values
AArma::AArma()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EfeitoMuzzle = nullptr;
	ImpactoSangue = nullptr;
	ImpactoGeral = nullptr;
	DecalImpactoGeral = nullptr;
	SomDoTiro = nullptr;

	MalhaDaArma = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MalhaDaArma"));

	RootComponent = MalhaDaArma;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshDaArma
	(TEXT("SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	if (MeshDaArma.Succeeded())
	{
		MalhaDaArma->SetSkeletalMesh(MeshDaArma.Object);
	}


	/* criamos um componente SETA (Arrow)*/
	SetaDaArma = CreateDefaultSubobject<UArrowComponent>(FName("SetaCanoDaArma"));
	/* solicitamos que esse construtor seja acoplado a MalhaDaArma
	contudo é solicitado um attach para algo que não foi configurado
	pois primeiro a classe c++ é executada e depois qualquer Blueprint
	que tem esta classe como pai. Assim o mesh ou skeletamesh de arma ainda
	não estará disponivel aqui pois está sendo setado na Blueprint
	logo precisamos configurar no código MalhaDaArma*/
	SetaDaArma->SetupAttachment(MalhaDaArma, FName("MuzzleFlashSocket"));
	SetaDaArma->AttachToComponent(MalhaDaArma, FAttachmentTransformRules::SnapToTargetIncludingScale, 
		FName("MuzzleFlashSocket"));
	SetaDaArma->SetRelativeLocation(FVector(1.5f, 0.f, -1.2f));
	SetaDaArma->SetRelativeScale3D(FVector(0.3f, 0.8f, 0.7f));
}

// Called when the game starts or when spawned
void AArma::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArma::Atirar()
{
	SetaDaArma = FindComponentByClass<UArrowComponent>();
	if (SetaDaArma != nullptr)
	{
		/* SetaDaArma é um componente do ator logo para achar sua localização
		utilizamos GetComponentLocation*/
		FVector Inicio = SetaDaArma->GetComponentLocation();
		FVector Direcao = SetaDaArma->GetComponentRotation().Vector();
		FVector Fim = Inicio + (Direcao * 1000);

		/* o raio de Raycast vai percorrer até o fim e precisa colocar
		as informações de HIT(impacto) em alguma estrutura. Essa estrutura
		é FHitResult */
		FHitResult InfoImpacto;
		FCollisionQueryParams Parametros;
		/* não queremos que o raio colida com a própria seta */
		Parametros.AddIgnoredActor(this);
		/* GetOwner devolve o dono da seta
		neste caso devolve a MalhaDaArma 
		também não queremos que a malha da arma seja considerada */
		Parametros.AddIgnoredActor(GetOwner());
		Parametros.bTraceComplex = true;

		bool AcertouEmAlgo = GetWorld()->LineTraceSingleByChannel(InfoImpacto,
			Inicio, Fim, ECollisionChannel::ECC_Visibility, Parametros);

		if (SomDoTiro)
		{
			UGameplayStatics::PlaySoundAtLocation(SetaDaArma, SomDoTiro, Inicio);
		}

		if (AcertouEmAlgo)
		{
			UE_LOG(LogTemp, Warning, TEXT("Acertou em algo"));
			/* qual o ator qua o raio impactou*/
			AActor* Ator = InfoImpacto.GetActor();
			/* se a classe do ator que o raio atingiu for do tipo SkeletalMeshActor
			ou subclasses dela entre neste if. ImpactoSangue precisa ser valido*/
			if (Ator->GetClass()->IsChildOf(ACharacter::StaticClass()) && ImpactoSangue)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactoSangue,
					InfoImpacto.Location, InfoImpacto.ImpactNormal.Rotation(), true);

				/* aqui tentamos fazer um cast para o objeto do tipo Player
				APersonagemTPS. Se o ponteiro Jogador não tiver vazio significa
				que o cast deu certo e que o trace do tiro colidiu com um
				jogador*/
				APersonagemTPS* Jogador = Cast<APersonagemTPS>(Ator);
				/* então vai entrar neste if e setar o Health dele - 0.25 a cada tiro*/
				if (Jogador != nullptr)
				{
					Jogador->SetHealth(10.f);
				}
				else
				{
					/* se não for um jogador o cast irá falhar e o ponteiro
					jogador será nulo, entrando no else
					
					novamente o cast, mas desta vez para o tipo da classe
					pertencente ao objeto inimigo*/
					ABotCharacter* Inimigo = Cast<ABotCharacter>(Ator);

					if (Inimigo != nullptr)
					{
						Inimigo->SetHealth(5.f);
					}
				}
			}
			/* se não for um inimigo humanoide não queremos que tenha sangue*/
			else if (ImpactoGeral)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactoGeral,
					InfoImpacto.Location, InfoImpacto.ImpactNormal.Rotation(), true);

				FVector TamanhoVariavel = FVector(FMath::RandRange(10.0f, 50.f));
				UGameplayStatics::SpawnDecalAttached(DecalImpactoGeral, TamanhoVariavel,
					InfoImpacto.GetComponent(), NAME_None, InfoImpacto.Location,
					InfoImpacto.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition,
					60.f);
			}
		}

		//DrawDebugLine(GetWorld(), Inicio, Fim, FColor::Red, false, 5.0f, (uint8)0, 1.0f);

		if (EfeitoMuzzle != nullptr)
		{
			FVector Localizacao = SetaDaArma->GetComponentLocation();
			FRotator Rotacao = SetaDaArma->GetComponentRotation();
			FVector Escala = FVector(0.9f);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfeitoMuzzle,
				Localizacao, Rotacao, Escala, true);
		}
	}
}

