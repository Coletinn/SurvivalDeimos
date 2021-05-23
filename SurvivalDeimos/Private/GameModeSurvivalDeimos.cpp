// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSurvivalDeimos.h"
#include "PersonagemTPS.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


void AGameModeSurvivalDeimos::BeginPlay()
{
	Super::BeginPlay();
	/* obtenho o personagem do jogo e coloco em player*/
	APersonagemTPS* Player = Cast<APersonagemTPS>
		(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	/* aqui se for setada algo como classe do HUD entramos*/
	if (PlayerHUDClass != nullptr)
	{
		/* widgetAtual passa a ser a widget criada (tela com health) */
		WidgetAtual = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		/* se for criada com sucesso a função addtoviewport é chamada*/
		if (WidgetAtual)
		{
			WidgetAtual->AddToViewport();
		}
	}
}