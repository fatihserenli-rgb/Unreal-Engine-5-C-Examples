// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayCycleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChanged, int32, Hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMorningStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoontStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEveningtStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNightStarted);

class ADirectionalLight;

UCLASS()
class STAJ_API ADayCycleManager : public AActor
{
    GENERATED_BODY()
    
public:
    ADayCycleManager();
    
    virtual void BeginPlay() override;
    
    virtual void Tick( float DeltaSeconds ) override;
    UPROPERTY(BlueprintAssignable)
    FOnHourChanged OnHourChanged;
    
    UPROPERTY(BlueprintAssignable)
    FOnMorningStarted OnMorningStarted;
    
    UPROPERTY(BlueprintAssignable)
    FOnNoontStarted OnNoontStarted;
    
    UPROPERTY(BlueprintAssignable)
    FOnEveningtStarted OnEveningtStarted;
    
    UPROPERTY(BlueprintAssignable)
    FOnNightStarted OnNightStarted;
    
private:
    UPROPERTY(EditAnywhere)
    ADirectionalLight *Sun;
    
    UPROPERTY(EditAnywhere)
    int32 DayLength;
    
    int8 Hour;
    int8 Minute;
};
