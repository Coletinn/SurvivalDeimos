// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Arma.h"
#include "PersonagemTPS.h"

ABotAIController::ABotAIController()
{
	/* construtor que cria o componente de sensores do bot inimigo*/
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(FName("PawnSensingComp"));
	/* deve fazer a varredura sensorial a cada 1/4 segundo (0.25f) */
	PawnSensingComp->SensingInterval = .25f;
	/* deve detectar apenas players */
	PawnSensingComp->bOnlySensePlayers = true;
	/* angulo de visão de 180 graus*/
	PawnSensingComp->SetPeripheralVisionAngle(180.f);
	/* raio de visão de 3000 unidades*/
	PawnSensingComp->SightRadius = 3000.f;

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComponent"));
	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackBoardComp"));
}

void ABotAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	/* se o sensor do inimigo disparar o evento OnSeePawn significa que ele
	detectou nosso jogador
	neste caso iremos amarrar a função*/
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ABotAIController::OnSeePawn);

	if (BehaviorTree)
	{
		/* iniciamos a BlackBoard e a behaviortree*/
		BlackBoardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*BehaviorTree);
		BlackBoardComp->SetValueAsBool("DevePerambular", true);
	}
}

void ABotAIController::OnSeePawn(APawn* SensedPawn)
{
	ABotCharacter* Bot = Cast<ABotCharacter>(GetPawn());
	APersonagemTPS* Player = Cast<APersonagemTPS>(SensedPawn);


	if (Bot->GetHealth() <= 0.0f || Player->IsEstaMorto())
	{
		BlackBoardComp->SetValueAsObject("Inimigo", nullptr);
		return;
	}
	
	/* se o health do bot for maior que zero e o player não estiver morto 
	continue e persiga e atire no player*/
	if (Bot->GetHealth() > 0.0f && !Player->IsEstaMorto())
		if (BlackBoardComp && SensedPawn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Estou te vendo"));

			/* aqui setamos a variável do BlackBoard inimigo com o valor
			do objeto SensedPawn: este objeto é retornado quando o game
			está em execução
			e o sensor do inimigo detecta seu inimigo*/
			BlackBoardComp->SetValueAsObject("Inimigo", SensedPawn);
			BlackBoardComp->SetValueAsBool("DevePerambular", false);
			/* estamos na classe Controller do bot(inimigo)
			precisamos obter uma referencia ao objeto Inimigo(BotCharacter)
			que está sendo possuido por este Controller(BotAIController)
			GetPawn() devolve esta referencia mas devolve via uma classe Genérica
			do tipo APawn. Como ACharacter é filha de APawn e BotCharacter é filho
			de ACharacter podemos especializar este retorno
			usamos Cast para transformar o objeto genérico retornado do tipo
			APawn em ABotCharacter */

			/* depois chamamos o metodo atirar da arma deste bot inimigo
			logo toda vez que estiver vendo o nosso player ele vai atirar
			com sensibilidade de .25 */
			Bot->ArmaInimigo->Atirar();
		}

}