// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SGameplayFunctionLibrary.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{
    Duration = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
    Super::StartAction_Implementation(Instigator);
    
    USActionComponent* ActionComp = GetOwningComponent();
    USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(ActionComp->GetOwner());
    if (AttributeComp)
    {
        AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::ApplyDamage);
    }
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
    Super::StopAction_Implementation(Instigator);

    USActionComponent* ActionComp = GetOwningComponent();
    USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(ActionComp->GetOwner());
    if (AttributeComp)
    {
        AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::ApplyDamage);
    }
}

void USActionEffect_Thorns::ApplyDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    AActor* Owner = GetOwningComponent()->GetOwner();

    if (Delta < 0.0f && Owner != InstigatorActor)
    {
        int32 Damage = FMath::RoundToInt(Delta * 0.2f);
        USGameplayFunctionLibrary::ApplyDamage(Owner, InstigatorActor, FMath::Abs(Damage)); // ApplyDamage takes a positive damage value not a net change
    }
}

