#include "VoxelWorld.h"
#include "VoxelChunk.h"
#include "ProceduralMeshComponent.h"
#include "PlanetCharacter.h"
#include "Kismet/GameplayStatics.h"

const float AVoxelWorld::VoxelSize = 25.0f;

AVoxelWorld::AVoxelWorld()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

AVoxelWorld::~AVoxelWorld()
{
    for (auto* Chunk : Chunks)
    {
        delete Chunk;
    }
}

void AVoxelWorld::BeginPlay()
{
    Super::BeginPlay();
    GenerateChunks();

    FVector PlanetCenter = GetActorLocation() + FVector(ChunksPerAxis * ChunkSize / 2) * VoxelSize;

    APlanetCharacter* Player = Cast<APlanetCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (Player)
    {
        Player->SetPlanetCenter(PlanetCenter);
    }
}

void AVoxelWorld::GenerateChunks()
{
    FVector PlanetCenter = GetActorLocation() + FVector(ChunksPerAxis * ChunkSize / 2) * VoxelSize;

    for (int32 x = 0; x < ChunksPerAxis; ++x)
    for (int32 y = 0; y < ChunksPerAxis; ++y)
    for (int32 z = 0; z < ChunksPerAxis; ++z)
    {
        FVector RealOrigin = GetActorLocation() + FVector(x, y, z) * (ChunkSize / 2) * VoxelSize;
        FVector MathOrigin = GetActorLocation() + FVector(x, y, z) * ChunkSize * VoxelSize;
        FVoxelChunk* NewChunk = new FVoxelChunk(ChunkSize, VoxelSize, RealOrigin, MathOrigin, PlanetCenter);
        NewChunk->GenerateVoxelData();
        Chunks.Add(NewChunk);

        UProceduralMeshComponent* Mesh = NewObject<UProceduralMeshComponent>(this);
        Mesh->RegisterComponent();
        Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Mesh->SetWorldLocation(RealOrigin);
        Meshes.Add(Mesh);

        NewChunk->BuildMesh(Mesh);
    }
}
