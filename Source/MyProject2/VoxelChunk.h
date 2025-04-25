#pragma once

#include "CoreMinimal.h"

class UProceduralMeshComponent;

class FVoxelChunk
{
public:
    FVoxelChunk(int32 InSize, float InVoxelSize, FVector InRealOrigin, FVector InMathOrigin, FVector InPlanetCenter);

    void GenerateVoxelData();
    void BuildMesh(UProceduralMeshComponent* MeshComponent);

private:
    int32 Size;
    float VoxelSize;
    FVector RealOrigin;
    FVector MathOrigin;
    FVector PlanetCenter;

    TArray<uint8> Voxels;

    FORCEINLINE int32 Index(int32 X, int32 Y, int32 Z) const;
    void CreateCubeAt(FVector Position, TArray<FVector>& Vertices, TArray<int32>& Triangles);
};