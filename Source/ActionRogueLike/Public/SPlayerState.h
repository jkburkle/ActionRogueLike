// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, InstigatorActor, float, NewCredits, float, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintCallable, Category = "Player State")
	static ASPlayerState* GetPlayerState(AActor* FromActor);

	// Sets default values for this component's properties
	ASPlayerState();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Player State")
	float Credits;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCreditsChanged(AActor* InstigatorActor, float NewCredits, float Delta);

public:	

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	bool ApplyCreditsChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Player State")
	float GetCredits() const;
	
};
