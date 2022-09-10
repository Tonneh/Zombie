
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zombie/Character/ZombieCharacterBot.h"
#include "ZombieSpawner.generated.h"

class AShooterGameMode;
class AZombieCharacterBot;
UCLASS()
class ZOMBIE_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AZombieSpawner();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AZombieCharacterBot> ZombieClass;

	UPROPERTY()
	AShooterGameMode* ShooterGameMode;

	UPROPERTY(EditAnywhere)
	int32 SpawnAmount = 10; 
	void SpawnActor(int32 AmountToSpawn); 
public:	
	virtual void Tick(float DeltaTime) override;

};
