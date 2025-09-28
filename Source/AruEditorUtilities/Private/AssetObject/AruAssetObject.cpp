// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetObject/AruAssetObject.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AruAssetObject)


UAruAssetObject::UAruAssetObject()
	: ReferencedObject(nullptr)
	, AssetName(NAME_None)
{
}

void UAruAssetObject::SetReferencedObject(UObject* InObject)
{
	ReferencedObject = InObject;
}

UObject* UAruAssetObject::GetReferencedObject() const
{
	return ReferencedObject;
}

void UAruAssetObject::SetAssetName(const FName& InName)
{
	AssetName = InName;
}

FName UAruAssetObject::GetAssetName() const
{
	return AssetName;
}

void UAruAssetObject::AddAssetTag(const FName& InTag)
{
	if (!AssetTags.Contains(InTag))
	{
		AssetTags.Add(InTag);
	}
}

const TArray<FName>& UAruAssetObject::GetAssetTags() const
{
	return AssetTags;
}
