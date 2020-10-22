// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax; // start out at max health
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	Health = FMath::Clamp(Health+Delta, 0.0f, HealthMax); // make sure health doesn't go negative

	// we don't have an instigator right now so we just say nullptr
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth()
{
	return Health == HealthMax;
}

bool USAttributeComponent::IsLowHealth()
{
	return Health <= 0.2f * HealthMax; // returns true if at less than 20% health
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	
	return false;
}

