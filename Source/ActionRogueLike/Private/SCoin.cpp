// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"
#include "SPlayerState.h"


ASCoin::ASCoin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

    CreditValue = 20;
}


void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	// get credits storage
    ASPlayerState* PState = ASPlayerState::GetPlayerState(InstigatorPawn);
    // if credits storage isnt null
    if (PState)
    {
        if (PState->ApplyCreditsChange(this, CreditValue))
        {
            HideAndCooldownPowerup();
        }
    }
}

