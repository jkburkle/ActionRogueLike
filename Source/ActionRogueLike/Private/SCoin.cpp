// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"


ASCoin::ASCoin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}


void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	// get credits storage
    // if credits storage isnt null
        // if successful credit deposit (add credits)
            // HideAndCooldownPowerup();
        // }
    // }
}

