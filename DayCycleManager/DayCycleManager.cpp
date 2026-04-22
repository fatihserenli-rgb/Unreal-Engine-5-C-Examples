#include "DayCycleManager.h"
#include "Engine/DirectionalLight.h"

ADayCycleManager::ADayCycleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	Hour = 12;
	Minute = 0.0f;
}

void ADayCycleManager::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.TickInterval = ((DayLength * 60.0f) / 360.0f) / 2;
}

void ADayCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Minute += 2;
	if (Minute >= 60)
	{
		Minute -= 60;
		Hour++;
		OnHourChanged.Broadcast(Hour);
		switch (Hour)
		{
		case 0:
			OnNightStarted.Broadcast();
			break;
		case 6:
			OnMorningStarted.Broadcast();
			break;
		case 12:
			OnNoontStarted.Broadcast();
			break;
		case 18:
			OnEveningtStarted.Broadcast();
			break;
		default:
			break;
		}
	}
	if (Hour >= 24)
	{
		Hour = 0;
	}
	UE_LOG(LogTemp, Log, TEXT("Hour: %d, Minute = %d"), Hour,Minute);
	Sun->AddActorLocalRotation(FRotator(0.5f, 0.0f, 0.0f));
}