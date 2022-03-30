// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

USTRUCT()
struct FTileInfo
{
	GENERATED_BODY()
	
	int Cost;
};

USTRUCT()
struct FTileColumn
{
	GENERATED_BODY()
	
	TArray<FTileInfo> Columns;
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

	TArray<FTileColumn> GridTile;

private:
	
	const float GetUsableTileSize() { return TileSize * 0.9f; }
	
	void RemoveAllDecals();
	
	UBoxComponent* BoxBounds;

	TArray<UDecalComponent*> Decals;
};
