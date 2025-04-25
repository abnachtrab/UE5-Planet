#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VoxelChunk.h"
#include "VoxelWorld.generated.h"

UCLASS()
class MYPROJECT2_API AVoxelWorld : public AActor
{
    GENERATED_BODY()

public:
    AVoxelWorld();
    ~AVoxelWorld();

protected:
    virtual void BeginPlay() override;

private:
    static const int32 ChunkSize = 16;
    static const int32 ChunksPerAxis = 4;
    static const float VoxelSize;

    TArray<FVoxelChunk*> Chunks;
    TArray<UProceduralMeshComponent*> Meshes;

    void GenerateChunks();
};