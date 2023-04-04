// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockHeadGameMode.h"
#include "BlockHeadGameInstance.h"
#include "../GluttonTools.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

void ABlockHeadGameMode::BeginPlay() {
	Super::BeginPlay();
	GameInstance = CastChecked<UBlockHeadGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance) 
	{
		GameInstance->SetInputMode(true);
	}
}

void ABlockHeadGameMode::LevelCompleted()
{
	GetWorldTimerManager().SetTimer(LevelSwapTimer, this, &ABlockHeadGameMode::NextLevel, 2.0f);

	if(DefaultLevelCompleteWidget)
	{
		LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelCompleteWidget);

		if(LevelCompleteWidget)
		{
			LevelCompleteWidget->AddToViewport();
		}		
	}
	else
	{
		GLUTTON_LOG("Default Level Complete Widget is not set.");
	}
}

void ABlockHeadGameMode::NextLevel()
{
	if(GameInstance->IsPlayerOnFinalLevel())
	{
		LevelCompleteWidget->RemoveFromParent();
		GameCompleted(true);
	}
	else
	{
		GameInstance->LoadNextLevel();
	}	
}

void ABlockHeadGameMode::GameCompleted(bool PlayerWon)
{
	GameInstance->SetInputMode(false);
	if (DefaultGameCompleteWidget)
	{
		GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultGameCompleteWidget);

		if (GameCompleteWidget)
		{
			GameCompleteWidget->AddToViewport();
			UTextBlock* LostOrComplete = Cast<UTextBlock>(GameCompleteWidget->GetWidgetFromName(TEXT("LostOrComplete")));
			const FText WinLossMessage = PlayerWon ? FText::FromString("You Won!") : FText::FromString("You Lost!");
			LostOrComplete->SetText(WinLossMessage);
		}
	}
	else
	{
		GLUTTON_LOG("Default Game Complete Widget is not set.");
	}
}

