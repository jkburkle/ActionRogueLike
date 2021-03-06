// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h" 
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
    SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
    Super::StartPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
    int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // possibly pass in player for kill instigator
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
    if (!CVarSpawnBots.GetValueOnGameThread())
    {
        UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
        return;
    }
    
    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
    if (ensure(QueryInstance))
    {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
    }
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus != EEnvQueryStatus::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
        return;
    }

    int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

    float MaxBotCount = 10.0f;

    if (DifficultyCurve)
    {
        MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
    }

    if (NrOfAliveBots >= MaxBotCount)
    {
        return;
    }
    
    TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

    if (Locations.IsValidIndex(0))
    {
        GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
    }
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
    if (ensure(Controller))
    {
        Controller->UnPossess();
        
        RestartPlayer(Controller);
    }
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
    ASCharacter* Player = Cast<ASCharacter>(VictimActor);
    if (Player)
    {
        FTimerHandle TimerHandle_RespawnDelay;

        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

        float RespawnDelay = 2.0f;
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
    }
    else
    {
        ASAICharacter* Minion = Cast<ASAICharacter>(VictimActor);
        Player = Cast<ASCharacter>(Killer); // we already declared player, and now we can guess that the player killed the minion
        if (Minion && Player) // it could be some other projectile that killed the minion, so we should be careful
        {
            // get credits storage
            ASPlayerState* PState = ASPlayerState::GetPlayerState(Player);
            // if credits storage isnt null
            if (PState)
            {
                PState->ApplyCreditsChange(this, 40); // hard coded to 40 credit reward for minions -- maybe will be a variable in the future for minion
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}
