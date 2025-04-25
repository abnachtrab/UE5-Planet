#include "VoxelChunk.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

FVoxelChunk::FVoxelChunk(int32 InSize, float InVoxelSize, FVector InRealOrigin, FVector InMathOrigin, FVector InPlanetCenter)
    : Size(InSize), VoxelSize(InVoxelSize), RealOrigin(InRealOrigin), MathOrigin(InMathOrigin), PlanetCenter(InPlanetCenter)
{
    Voxels.SetNum(Size * Size * Size);
}

void FVoxelChunk::GenerateVoxelData()
{
    float Radius = (Size * 4 * 0.5f) * VoxelSize * 0.95f;

    for (int32 x = 0; x < Size; ++x)
    for (int32 y = 0; y < Size; ++y)
    for (int32 z = 0; z < Size; ++z)
    {
        FVector Pos = MathOrigin + FVector(x, y, z) * VoxelSize;
        float Dist = FVector::Dist(Pos, PlanetCenter);

        float Height = Radius + FMath::PerlinNoise3D(Pos / 300.0f) * 150.0f;
        Voxels[x + Size * (y + Size * z)] = (Dist < Height) ? 1 : 0;
    }
}

void FVoxelChunk::BuildMesh(UProceduralMeshComponent* MeshComponent)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 x = 0; x < Size; ++x)
    for (int32 y = 0; y < Size; ++y)
    for (int32 z = 0; z < Size; ++z)
    {
        if (Voxels[x + Size * (y + Size * z)] == 0) continue;

        bool Exposed = true; // Temporarily disable culling at chunk edges

        if (!Exposed) continue;

        FVector Pos = RealOrigin + FVector(x, y, z) * VoxelSize;
        CreateCubeAt(Pos, Vertices, Triangles);
    }

    int32 NumVerts = Vertices.Num();
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    Normals.Init(FVector(0, 0, 1), NumVerts);
    UVs.Init(FVector2D(0, 0), NumVerts);
    VertexColors.Init(FLinearColor::White, NumVerts);
    Tangents.Init(FProcMeshTangent(1, 0, 0), NumVerts);

    MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void FVoxelChunk::CreateCubeAt(FVector Position, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    static const FVector CubeVerts[8] = {
        FVector(0, 0, 0), FVector(0, 0, 1),
        FVector(0, 1, 0), FVector(0, 1, 1),
        FVector(1, 0, 0), FVector(1, 0, 1),
        FVector(1, 1, 0), FVector(1, 1, 1)
    };

    static const int32 Faces[6][4] = {
        {0,2,6,4}, {1,5,7,3}, {0,4,5,1},
        {2,3,7,6}, {0,1,3,2}, {4,6,7,5}
    };

    int32 StartIndex = Vertices.Num();
    for (int i = 0; i < 8; i++)
        Vertices.Add(Position + (CubeVerts[i] - FVector(0.5f)) * VoxelSize);

    for (int f = 0; f < 6; f++)
    {
        Triangles.Append({
            StartIndex + Faces[f][0],
            StartIndex + Faces[f][1],
            StartIndex + Faces[f][2],
            StartIndex + Faces[f][0],
            StartIndex + Faces[f][2],
            StartIndex + Faces[f][3],
        });
    }
}
