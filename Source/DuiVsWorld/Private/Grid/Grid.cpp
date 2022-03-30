#include "Grid/Grid.h"

#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Decals = TArray<UDecalComponent*>();
	Tile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	BoxBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBoundsComponent"));
	Tile->SetMobility(EComponentMobility::Static);
	Tile->SetCollisionProfileName(TEXT("BlockAll"));
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void AGrid::Initialize(const float DecalTTL)
{
	Tile->ClearInstances();
	RemoveAllDecals();
	
	BoxBounds->InitBoxExtent(FVector(SizeX / 2.0f, SizeY / 2.0f, Height / 2.0f));
	BoxBounds->SetWorldLocation(GetActorLocation());
	
	const float DeltaSizeX = (SizeX - TileSize) / 2.0f;
	const float DeltaSizeY = (SizeY - TileSize) / 2.0f;
	
	for(int X = 0; X < (SizeX / TileSize); X++)
	{
		for(int Y = 0; Y < (SizeY / TileSize); Y++)
		{
			const FVector Start = GetActorLocation() + FVector((X * TileSize) - DeltaSizeX, (Y * TileSize) - DeltaSizeY,Height / 2.0f);
			const FVector End = GetActorLocation() + FVector((X * TileSize) - DeltaSizeX, (Y * TileSize) - DeltaSizeY,Height / -2.0f);
			
			FHitResult HitInfo;
			
			bool Hit = GetWorld()->LineTraceSingleByChannel(HitInfo, Start, End, ECC_Visibility/*ECC_GameTraceChannel3*/);
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5, 0, 1);
			if(Hit)
			{
				const float DecalTileSize = GetUsableTileSize() / 2.0f;
				const FRotator NormalHitRotator = HitInfo.ImpactNormal.Rotation();
				const FRotator DecalRotation = FRotator(NormalHitRotator.Pitch, GetActorRotation().Yaw, NormalHitRotator.Roll);
				
				UDecalComponent* TileDecal = UGameplayStatics::SpawnDecalAttached(TileMaterial, FVector(DecalTileSize),
					HitInfo.GetComponent(), HitInfo.BoneName,HitInfo.ImpactPoint, DecalRotation,
					EAttachLocation::KeepWorldPosition, DecalTTL);
				Decals.Add(TileDecal);
			}
			else if (bGenerateDefaultTile)
			{
				Tile->AddInstance(FTransform(FVector((X * TileSize) - DeltaSizeX, (Y * TileSize) - DeltaSizeY,0.0f)));
			}
		}
	}
}

void AGrid::RemoveAllDecals()
{
	for (UDecalComponent* Decal : Decals)
	{
		Decal->DestroyComponent();
	}
	Decals.Empty();
}

