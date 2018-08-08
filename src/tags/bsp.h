/*
	Project: haloforge
	File: bsp.h
	Copyright � 2009 SilentK, Abyll

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once

#include "map.h"

class CBSPInfoPrimary;
class CBSPInfoSecondary;
class CBSPHeader;
class CCollBSPHeader;
class CCollNode;
class CCollPlane;
class CCollLeaf;
class CCollBSP2DRef;
class CCollBSP2DNodes;
class CCollSurfaces;
class CCollEdges;
class CCollVertex;
class CShader;
class CNodes;
class CLeaves;
class CLeafSurface;
class CSubmeshTriIndices;
class CSubmeshHeader;
class CCluster;
class CWeather;

class CMaterialSubmeshHeader;

template<class T>
class CBSPReflexive
{
public:
	DWORD Count;
	T *Offset;
	DWORD Zeros;
};

// Place holder for unwritten classes
class BSPFiller
{
public:
};

class CBSPInfoPrimary
{
public:
	unsigned long BspMapOffset;
	unsigned long BspSize;
	CBSPInfoSecondary * BSPInfoSecondary;
	unsigned long Unknown;
	char bsptag[4]; // 'psbs'
	tag_name * TName;
	unsigned long Unknown1;
	short TagIndex;
	short TagID;
};

class CBSPInfoSecondary
{
public:
	CBSPHeader * BSPHeader;
	long Unknown[4];
	char bsptag[4]; // 'psbs'
};

class CBSPHeader
{
public:
  tagref LightmapsTag;
  unsigned long Unknown[37];
  CBSPReflexive<CShader> Shaders;
  CBSPReflexive<CCollBSPHeader> CollBspHeader;
  CBSPReflexive<CNodes> Nodes;
  unsigned long Unknown1[6];
  CBSPReflexive<CLeaves> Leaves;
  CBSPReflexive<CLeafSurface> LeafSurfaces;
  CBSPReflexive<CSubmeshTriIndices> SubmeshTriIndices;
  CBSPReflexive<CSubmeshHeader> SubmeshHeader;
  CBSPReflexive<BSPFiller> Chunk10;
  CBSPReflexive<BSPFiller> Chunk11;
  CBSPReflexive<BSPFiller> Chunk12;
  CBSPReflexive<CCluster> Clusters;
  unsigned long ClusterDataSize;
  unsigned long Unknown2;
  CBSPReflexive<BSPFiller> Chunk14;
  CBSPReflexive<BSPFiller> ClusterPortals;
  CBSPReflexive<BSPFiller> Chunk16a;
  CBSPReflexive<BSPFiller> BreakableSurfaces;
  CBSPReflexive<BSPFiller> FogPlanes;
  CBSPReflexive<BSPFiller> FogRegions;
  CBSPReflexive<BSPFiller> FogOrWeatherPallette;
  CBSPReflexive<BSPFiller> Chunk16f;
  CBSPReflexive<BSPFiller> Chunk16g;
  CBSPReflexive<CWeather> Weather;
  CBSPReflexive<BSPFiller> WeatherPolyhedra;
  CBSPReflexive<BSPFiller> Chunk19;
  CBSPReflexive<BSPFiller> Chunk20;
  CBSPReflexive<BSPFiller> PathfindingSurface;
  CBSPReflexive<BSPFiller> Chunk24;
  CBSPReflexive<BSPFiller> BackgroundSound;
  CBSPReflexive<BSPFiller> SoundEnvironment;
  unsigned long SoundPASDataSize;
  unsigned long Unknown3;
  CBSPReflexive<BSPFiller> Chunk25;
  CBSPReflexive<BSPFiller> Chunk26;
  CBSPReflexive<BSPFiller> Chunk27;
  CBSPReflexive<BSPFiller> Markers;
  CBSPReflexive<BSPFiller> DetailObjects;
  CBSPReflexive<BSPFiller> RuntimeDecals;
  unsigned long Unknown4[9];
}; // Size = 0x0288 ( 648 )

// **** BSP COLLISION LUMPS START ****

class CCollBSPHeader
{
public:
	CBSPReflexive<CCollNode> Nodes;        //12 bytes
	CBSPReflexive<CCollPlane> Planes;       //16 bytes
	CBSPReflexive<CCollLeaf> Leaves;       //8 bytes
	CBSPReflexive<CCollBSP2DRef> Bsp2dRef;     //8 bytes
	CBSPReflexive<CCollBSP2DNodes> Bsp2dNodes;   //20 bytes
	CBSPReflexive<CCollSurfaces> Surfaces;     //12 bytes
	CBSPReflexive<CCollEdges> Edges;        //24 bytes
	CBSPReflexive<CCollVertex> Vertices;     //16 bytes
};

class CCollNode
{
public:
	int PlaneIndex;
	int Back;
	int Front;
};

class CCollPlane
{
	float x;
	float y;
	float z;
	float d;
};

class CCollLeaf
{
	int Unknown[2];
};

class CCollBSP2DRef
{
	int Unknown[2];
};

class CCollBSP2DNodes
{
	float Unknown[3];
	unsigned int  Unknown2;
	unsigned int  LeafIndex;
};

class CCollSurfaces
{
	int Unknown[2];
	short Unknown1;
	short Unknown2;
};

class CCollEdges
{
	unsigned int Unknown[6];
};

class CCollVertex
{
	float Unknown[4];
};

// **** BSP COLLISION LUMPS END ****

// **** BSP LUMPS START ****

class CShader
{
public:

	tagref ShaderTag;
	unsigned short Unknown[2];

};

class CNodes
{
public:

  short Unknown[3];

};

class CLeaves
{
public:

  unsigned int Unknown[4];

};

class CLeafSurface
{
public:

  unsigned int Unknown[2];

};

class CSubmeshTriIndices
{
public:

  unsigned short tri_ind[3];

};

class CSubmeshHeader
{
public:

	short LightmapIndex;
	short Unknown;
	unsigned int Unknown1[4];
	CBSPReflexive<CMaterialSubmeshHeader> Material;

};

class CCluster
{
public:

	short SkyIndex;
	short FogIndex;
	short BackgroundSoundIndex;
	short SoundEnvIndex;
	short WeatherIndex;
	short TransitionBsp;
	unsigned int  Unknown1[10];
	CBSPReflexive<BSPFiller> SubCluster;
	unsigned int  Unknown2[7];
	CBSPReflexive<BSPFiller> Portals;

};

class CWeather
{
public:

	char Name[32];
	char tag[4];
	UINT NamePtr;
	UINT zero1;
	UINT TagId;
	UINT reserved[20];
	char tag2[4];
	UINT NamePtr2;
	UINT zero2;
	UINT signature2;
	UINT unk[24];

};

class CMaterialSubmeshHeader
{
public:

  tagref ShaderTag;
  UINT UnkZero2;
  UINT VertIndexOffset;
  UINT VertIndexCount;
  float Centroid[3];
  float AmbientColor[3];
  UINT DistLightCount;
  float DistLight1[6];
  float DistLight2[6];
  float unkFloat2[3];
  float ReflectTint[4];
  float ShadowVector[3];
  float ShadowColor[3];
  float Plane[4];
  UINT UnkFlag2;
  UINT UnkCount1;
  UINT VertexCount1;
  UINT UnkZero4;
  UINT VertexOffset;
  UINT Vert_Reflexive;
  UINT UnkAlways3;
  UINT VertexCount2;
  UINT UnkZero9;
  UINT UnkLightmapOffset;
  UINT CompVert_Reflexive;
  UINT UnkZero5[2];
  UINT SomeOffset1;
  UINT PcVertexDataOffset;
  UINT UnkZero6;
  UINT CompVertBufferSize;
  UINT UnkZero7;
  UINT SomeOffset2;
  UINT VertexDataOffset;
  UINT UnkZero8;

};

// **** BSP LUMPS END ****
