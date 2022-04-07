// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

struct FTile
{
public:
	FTile()
	{
		DecalComponent = nullptr;
	}

	FTile(int cost, FVector location, UDecalComponent* decalComponent)
	{
		Cost = cost;
		Location = location;
		DecalComponent = decalComponent;
	}

	void SetVisibility(bool Visibility)
	{
		if (DecalComponent)
		{
			DecalComponent->SetVisibility(Visibility);
		}
	}
	int Cost = 1;
	FVector Location;
	UDecalComponent* DecalComponent;
};

UCLASS()
class DUIVSWORLD_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrid();

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* Tile = nullptr;

	UFUNCTION()
	void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere)
	float TileSize = 100.0f;
	
	UPROPERTY(EditAnywhere)
	float SizeX;
	
	UPROPERTY(EditAnywhere)
	float SizeY;

	/** Height to start raycasting in cm */
	UPROPERTY(EditAnywhere)
	float Height;
	
	UPROPERTY(EditAnywhere)
	bool bGenerateDefaultTile;

	UFUNCTION(BlueprintCallable)
	void Initialize(const float DecalTTL = 0.0f);
	
	UPROPERTY(EditDefaultsOnly)
	UMaterial* TileMaterial;

private:
	const float GetUsableTileSize() { return TileSize * 0.9f; }
	
	void RemoveAllDecals();
	
	void EmptyGrid();
	
	UBoxComponent* BoxBounds = nullptr;

	TArray<UDecalComponent*> Decals;

	TArray<FTile> Grid;
};
