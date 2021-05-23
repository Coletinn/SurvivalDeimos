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

	/* função responsavel por dar o controle do Pawn(inimigo)
	ao controller. Ou seja, BotCharacter será controlado por ABot
	AIController quando for possuido por este controlador*/
	virtual void OnPossess(APawn* InPawn) override;

	/* o SensingComponent encapsula configurações e funcionalidades
	sensoriais(visão audição) de um ator.*/
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
