#pragma once

#include "CoreMinimal.h"
#include "AruTypes.h"
#include "AruFunctionLibrary.generated.h"

struct FAruActionDefinition;

struct FAruPropertyContext
{
	FProperty* PropertyPtr		= nullptr;
	TOptional<void*> ValuePtr	= {};

	FAruPropertyContext() : PropertyPtr (nullptr), ValuePtr ({}) {}
	FAruPropertyContext(FProperty* InPropertyPtr, void* InValuePtr) : PropertyPtr (InPropertyPtr), ValuePtr (TOptional<void*>{InValuePtr}) {}

	FORCEINLINE bool IsValid() const { return PropertyPtr != nullptr && ValuePtr.IsSet(); }
};

struct FAruProcessingParameters
{
	const FInstancedPropertyBag&					Parameters;
	const int32										RemainTime;
	const TFunction<bool(const FProperty*, void*)>&	PropertyProcessor;

	FAruProcessingParameters() = delete;
	FAruProcessingParameters(
		const TFunction<bool(const FProperty*, void*)>&	InProcessor,
		const FInstancedPropertyBag&					InParameters,
		const int32										InRemainTime)
			: Parameters(InParameters), RemainTime(InRemainTime), PropertyProcessor(InProcessor){};

	FAruProcessingParameters GetSubsequentParameters() const
	{
		return {PropertyProcessor, Parameters, RemainTime-1};
	} 
};

UCLASS()
class ARUEDITORUTILITIES_API UAruFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	static bool ModifySelectedAssets(const TArray<FAruActionDefinition>& Actions, const FAruProcessConfig& Configs);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static bool ModifyAssets(const TArray<UObject*>& AssetsToModify, const TArray<FAruActionDefinition>& Actions, const FAruProcessConfig& Configs);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static bool ValidateSelectedAssets(const TArray<FAruValidationDefinition>& Validations, const FAruProcessConfig& Configs);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static bool ValidateAssets(const TArray<UObject*>& AssetsToValidate, const TArray<FAruValidationDefinition>& Validations, const FAruProcessConfig& Configs);

	static bool ProcessAssets(
		const TArray<UObject*>& Objects,
		const FAruProcessConfig& Configs,
		const TFunction<bool(const FProperty*, void*)>&	PropertyProcessor);

	static bool ProcessAsset(
		UObject* const Object,
		const FAruProcessConfig& Configs,
		const TFunction<bool(const FProperty*, void*)>&	PropertyProcessor);

	static FAruPropertyContext FindPropertyByPath(
		const FProperty* InProperty,
		const void* InPropertyValue,
		const FString& Path);

	static FAruPropertyContext FindPropertyByPath(
		const UStruct* InStructType,
		const void* InStructValue,
		const FString& Path);

	static const FProperty* FindPropertyByName(
		const UStruct* InStruct, 
		const FString& DisplayName);
		
	static FAruPropertyContext FindPropertyByChain(
		const FProperty* InProperty,
		const void* InPropertyValue,
		const TArrayView<FString> PropertyChain);

	static bool ProcessContainerValues(
		FProperty* PropertyPtr,
		void* ValuePtr,
		const FAruProcessingParameters& InParameters);

	static FString ResolveParameterizedString(const FInstancedPropertyBag& InParameters, const FString& SourceString);

	/**
	 * Determine if an AssetObject's tags match with a Definition's tags
	 * @param AssetObject The asset object containing tags to check
	 * @param Tags The tags to check against
	 * @return True if any tag in the AssetObject matches any tag in the Definition, or if either tag list is empty
	 */
	UFUNCTION(BlueprintPure, Category = "Aru Editor Utilities")
	static bool IsTagMatching(const class UAruAssetObject* AssetObject, const TArray<FName>& Tags);
};
