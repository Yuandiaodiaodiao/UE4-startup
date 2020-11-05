// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerController.h"

UTowerController::UTowerController()
{
    UE_LOG(LogTemp,Error,TEXT("TowerController constructor1"));
}

void UTowerController::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    UE_LOG(LogTemp,Error,TEXT("TowerController Serialize FArchive"));
}


// void UTowerController::Serialize(FStructuredArchive::FRecord Record)
// {
//     Super::Serialize(Record);
//     
//     UE_LOG(LogTemp,Error,TEXT("TowerController Serialize2"));
//
// }
FArchive& operator<<(FArchive& Ar, UTowerController* SaveActorRef)
{
    
    UE_LOG(LogTemp,Error,TEXT("UTowerController* Serialize operator<<"));
    return Ar;
}
