// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SCharacter.h"

ASPlayerState::ASPlayerState()
{
    Credits = 0;
}

ASPlayerState* ASPlayerState::GetPlayerState(AActor* FromActor)
{
	ASCharacter* Player = Cast<ASCharacter>(FromActor);
    if (Player)
	{
		return Cast<ASPlayerState>(Player->GetController()->PlayerState);
	}
	
	return nullptr;
}

bool ASPlayerState::ApplyCreditsChange(AActor* InstigatorActor, float Delta)
{
    Credits += Delta; // (no clamping here, I don't think there's a max Credit value)

	OnCreditsChanged.Broadcast(InstigatorActor, Credits, Delta);

    UE_LOG(LogTemp, Log, TEXT("Credits changed: %f"), Credits);

	return true; // not much complex behavior yet
}

float ASPlayerState::GetCredits() const
{
    return Credits;
}

