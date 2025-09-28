// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetCollector/AruAssetCollector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AruAssetCollector)


UAruAssetCollector::UAruAssetCollector()
{
	// Initialize any necessary resources
}

UAruAssetCollector::~UAruAssetCollector()
{
	// Ensure proper cleanup of asset objects when this collector is destroyed
	ClearAssets();
}

void UAruAssetCollector::Collect_Implementation()
{
	// Base implementation does nothing
	// Derived classes should override this function to implement their own collection logic
}

TArray<UAruAssetObject*> UAruAssetCollector::Get() const
{
	TArray<UAruAssetObject*> Result;
	for (TObjectPtr<UAruAssetObject> AruAssetObject : AruAssetObjects)
	{
		Result.Add(AruAssetObject.Get());
	}
	return Result;
}

void UAruAssetCollector::ClearAssets()
{
	AruAssetObjects.Empty();
}

void UAruAssetCollector::AddAruAssetObject(UAruAssetObject* AruAssetObject)
{
	if (AruAssetObject)
	{
		AruAssetObjects.AddUnique(AruAssetObject);
	}
}

UAruAssetObject* UAruAssetCollector::AddInstance(UObject* SourceObject, const TArray<FName>& Tags)
{
	if (!SourceObject)
	{
		return nullptr;
	}

	// Create a new asset object instance
	UAruAssetObject* NewAssetObject = NewObject<UAruAssetObject>(this);
	
	// Set the referenced object
	NewAssetObject->SetReferencedObject(SourceObject);
	
	// Set the asset name based on the object name if available
	FName ObjectName = NAME_None;
	if (SourceObject->GetName().Len() > 0)
	{
		ObjectName = FName(*SourceObject->GetName());
	}
	else if (SourceObject->GetClass())
	{
		ObjectName = FName(*FString::Printf(TEXT("%s_Instance"), *SourceObject->GetClass()->GetName()));
	}
	NewAssetObject->SetAssetName(ObjectName);
	
	// Add all tags
	for (const FName& Tag : Tags)
	{
		NewAssetObject->AddAssetTag(Tag);
	}
	
	// Add the object to our collection
	AddAruAssetObject(NewAssetObject);
	
	return NewAssetObject;
}
