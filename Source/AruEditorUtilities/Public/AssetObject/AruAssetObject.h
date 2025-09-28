// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AruAssetObject.generated.h"

/**
 * Class that maintains a reference to a UObject and an FName
 */
UCLASS(Blueprintable)
class ARUEDITORUTILITIES_API UAruAssetObject : public UObject
{
	GENERATED_BODY()

public:
	UAruAssetObject();

	/** Set the referenced object */
	UFUNCTION(BlueprintCallable, Category = "Asset Object")
	void SetReferencedObject(UObject* InObject);

	/** Get the referenced object */
	UFUNCTION(BlueprintPure, Category = "Asset Object")
	UObject* GetReferencedObject() const;

	/** Set the asset name */
	UFUNCTION(BlueprintCallable, Category = "Asset Object")
	void SetAssetName(const FName& InName);

	/** Get the asset name */
	UFUNCTION(BlueprintPure, Category = "Asset Object")
	FName GetAssetName() const;

	/** Add an asset tag to the tags array, if it doesn't exist already */
	UFUNCTION(BlueprintCallable, Category = "Asset Object")
	void AddAssetTag(const FName& InTag);

	/** Get all asset tags */
	UFUNCTION(BlueprintPure, Category = "Asset Object")
	const TArray<FName>& GetAssetTags() const;

protected:
	/** The object being referenced */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Object")
	TObjectPtr<UObject> ReferencedObject;

	/** The name of this asset object */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Object")
	FName AssetName;

	/** The tags associated with this asset object */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Object")
	TArray<FName> AssetTags;
};
