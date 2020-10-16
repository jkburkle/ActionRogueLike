// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* MyController = OwnerComp.GetAIOwner();
    if (ensure(MyController))
    {
        ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
        if (MyPawn == nullptr)
        {
            return EBTNodeResult::Failed;
        }

        USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp)
        {
            if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		    {
			    return EBTNodeResult::Succeeded;
		    }
        }
    }

    return EBTNodeResult::Failed;
}

