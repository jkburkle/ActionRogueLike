// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"
#include "AIController.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // check distance between AI pawn and target actor

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackboardComp))
    {
        AAIController* MyController = OwnerComp.GetAIOwner();
        if (ensure(MyController))
        {
            APawn* AIPawn = MyController->GetPawn();
            if (ensure(AIPawn))
            {
                bool bIsLowHealth = false;
                
                USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
                if (AttributeComp)
                {
                    bIsLowHealth = AttributeComp->IsLowHealth();
                }

                BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
            }
        }
    }
}