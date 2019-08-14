// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TODO protect against empty patrol routes

	// Get the patrol points
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensureAlways(PatrolRoute)) { return EBTNodeResult::Failed; }
	
	//auto AIController = OwnerComp.GetAIOwner();
	//auto ControlledPawn = AIController->GetPawn();
	//auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	//auto PatrolPoints = PatrollingGuard->PatrolPointsCPP;
	
	// Warn about empty patrol routes
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
		return EBTNodeResult::Failed;
	}
	
	// Set next waypoint

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	// Cycle the index
	auto NextIndex = (1 + Index) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	return EBTNodeResult::Succeeded;
}