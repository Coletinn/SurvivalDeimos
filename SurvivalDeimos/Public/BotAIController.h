// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALDEIMOS_API ABotAIController : public AAIController
{
	GENERATED_BODY()

protected:
	ABotAIController();

	/* fun��o responsavel por dar o controle do Pawn(inimigo)
	ao controller. Ou seja, BotCharacter ser� controlado por ABot
	AIController quando for possuido por este controlador*/
	virtual void OnPossess(APawn* InPawn) override;

	/* o SensingComponent encapsula configura��es e funcionalidades
	sensoriais(vis�o audi��o) de um ator.*/
	UPROPERTY(EditDefaultsOnly)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(EditDefaultsOnly)
	class UBlackboardComponent* BlackBoardComp;
	
	UFUNCTION()
	void OnSeePawn(class APawn* SensedPawn);
};
