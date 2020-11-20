// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_MeleeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributecomponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"
#include "SGameplayFunctionLibrary.h"


USBTTask_MeleeAttack::USBTTask_MeleeAttack()
{
    AttackAnimDelay = 1.0f;
    Range = 270.0f;
    Damage = 30.0f;
}

EBTNodeResult::Type USBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* MyController = OwnerComp.GetAIOwner();
    if (ensure(MyController))
    {
        ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
        if (MyPawn == nullptr)
        {
            return EBTNodeResult::Failed;
        }

        AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
        if (TargetActor == nullptr)
        {
            return EBTNodeResult::Failed;
        }

        if (!USAttributeComponent::IsActorAlive(TargetActor))
        {
            return EBTNodeResult::Failed;
        }

        MyPawn->PlayAnimMontage(AttackAnim);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", MyPawn);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

void USBTTask_MeleeAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
    // Ignore Attacker
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);
    
    FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    FVector EyeLocation;
	FRotator EyeRotation;
	InstigatorCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    FVector End = EyeLocation + (EyeRotation.Vector() * Range);

    TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(30.0f);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape, Params);

    //FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

    for (FHitResult Hit : Hits)
	{
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.0f, 32, LineColor, false, 2.0f);

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			USGameplayFunctionLibrary::ApplyDamage(InstigatorCharacter, HitActor, Damage);
            break;
		}
	}

    //DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

