// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SCoin.generated.h"


class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	float CreditValue;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	ASCoin();
	
};
