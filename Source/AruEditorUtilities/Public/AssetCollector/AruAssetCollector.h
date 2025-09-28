// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AssetObject/AruAssetObject.h"
#include "AruAssetCollector.generated.h"

/**
 * Base class for asset collection that maintains a pool of AruAssetObject instances
 * Can be extended in Blueprint to implement custom asset collection logic
 */
UCLASS(Blueprintable, Abstract)
class ARUEDITORUTILITIES_API UAruAssetCollector : public UObject
{
	GENERATED_BODY()

public:
	UAruAssetCollector();
	virtual ~UAruAssetCollector();

	/**
	 * Collect assets and populate the AruAssetObjects array
	 * Blueprint implementable function that can be overridden in child classes
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Asset Collector")
	void Collect();
	virtual void Collect_Implementation();

	/**
	 * Returns all collected asset objects
	 * @return Array of all collected AruAssetObjects
	 */
	UFUNCTION(BlueprintPure, Category = "Asset Collector")
	TArray<UAruAssetObject*> Get() const;

	/**
	 * Clear all collected assets
	 */
	UFUNCTION(BlueprintCallable, Category = "Asset Collector")
	virtual void ClearAssets();
	
	/**
	 * Create a new AruAssetObject instance with the provided object reference and tags
	 * @param SourceObject The UObject reference to associate with the asset object
	 * @param Tags Array of tags to add to the asset object
	 * @return The newly created asset object
	 */
	UFUNCTION(BlueprintCallable, Category = "Asset Collector")
	UAruAssetObject* AddInstance(UObject* SourceObject, const TArray<FName>& Tags);

protected:
	/**
	 * Add an asset object to the collection
	 * @param AruAssetObject The asset object to add
	 */
	UFUNCTION(BlueprintCallable, Category = "Asset Collector")
	void AddAruAssetObject(UAruAssetObject* AruAssetObject);

	/** The collection of asset objects */
	UPROPERTY(BlueprintReadOnly, Category = "Asset Collector")
	TArray<TObjectPtr<UAruAssetObject>> AruAssetObjects;
};
