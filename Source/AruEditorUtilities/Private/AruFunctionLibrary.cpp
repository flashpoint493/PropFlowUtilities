#include "AruFunctionLibrary.h"
#include "AruTypes.h"
#include "EditorUtilityLibrary.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "AssetObject/AruAssetObject.h"
#include "AssetFilters/AruFilter_Proxy.h"
#include "AssetPredicates/AruPredicate_Proxy.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AruFunctionLibrary)

#define LOCTEXT_NAMESPACE "AruEditorUtilities"
/**
 * Initialize all proxy instances in the structure
 * @param InStructPtr - Pointer to the structure to be processed
 * @param InOwner - Owner object to be used when creating proxy instances
 */
template<typename T>
void InitializeStructProxies(const T* InStructPtr, UObject* InOwner)
{
	if (!InStructPtr)
	{
		return;
	}

	// Check if the structure is of type BlueprintProxy and initialize it
	if constexpr (std::is_base_of<FAruFilter_BlueprintProxy, T>::value)
	{
		// Use const_cast because we need to modify it
		FAruFilter_BlueprintProxy* MutableProxy = const_cast<FAruFilter_BlueprintProxy*>(static_cast<const FAruFilter_BlueprintProxy*>(InStructPtr));
		MutableProxy->InitializeProxy(InOwner);
	}
	else if constexpr (std::is_base_of<FAruPredicate_BlueprintProxy, T>::value)
	{
		// Use const_cast because we need to modify it
		FAruPredicate_BlueprintProxy* MutableProxy = const_cast<FAruPredicate_BlueprintProxy*>(static_cast<const FAruPredicate_BlueprintProxy*>(InStructPtr));
		MutableProxy->InitializeProxy(InOwner);
	}
}

// Function specifically for initializing FAruActionDefinition
void InitializeActionProxies(const TArray<FAruActionDefinition>& Definitions)
{
	// Use GetTransientPackage() as the Owner
	UObject* Owner = GetTransientPackage();

	// Process each definition in the array
	for (const auto& Definition : Definitions)
	{
		// Initialize filters for the current definition
		for (const auto& ConditionStruct : Definition.ForEachCondition())
		{
			// Use static type checking instead of dynamic_cast
			// If ConditionStruct is of type FAruFilter_BlueprintProxy, this conversion is safe
			// Otherwise, an incorrect pointer would be obtained, but checking the actual type name of ConditionStruct ensures safety
			const FName StructTypeName = ConditionStruct.StaticStruct()->GetFName();
			if (StructTypeName == FAruFilter_BlueprintProxy::StaticStruct()->GetFName())
			{
				// Safe conversion, as we have confirmed the type
				const FAruFilter_BlueprintProxy& BlueprintProxy = static_cast<const FAruFilter_BlueprintProxy&>(ConditionStruct);
				InitializeStructProxies(&BlueprintProxy, Owner);
			}
		}

		// Initialize predicates for the current definition
		for (const auto& PredicateStruct : Definition.ForEachPredicates())
		{
			// Use static type checking instead of dynamic_cast
			const FName StructTypeName = PredicateStruct.StaticStruct()->GetFName();
			if (StructTypeName == FAruPredicate_BlueprintProxy::StaticStruct()->GetFName())
			{
				// Safe conversion, as we have confirmed the type
				const FAruPredicate_BlueprintProxy& BlueprintProxy = static_cast<const FAruPredicate_BlueprintProxy&>(PredicateStruct);
				InitializeStructProxies(&BlueprintProxy, Owner);
			}
		}
	}
}

// Function specifically for initializing FAruValidationDefinition
void InitializeValidationProxies(const TArray<FAruValidationDefinition>& Validations)
{
	// Use GetTransientPackage() as the Owner
	UObject* Owner = GetTransientPackage();

	// Process each validation definition in the array
	for (const auto& Validation : Validations)
	{
		// Initialize filters for the current validation definition
		for (const auto& ConditionStruct : Validation.ForEachCondition())
		{
			// Use static type checking instead of dynamic_cast
			const FName StructTypeName = ConditionStruct.StaticStruct()->GetFName();
			if (StructTypeName == FAruFilter_BlueprintProxy::StaticStruct()->GetFName())
			{
				// Safe conversion, as we have confirmed the type
				const FAruFilter_BlueprintProxy& BlueprintProxy = static_cast<const FAruFilter_BlueprintProxy&>(ConditionStruct);
				InitializeStructProxies(&BlueprintProxy, Owner);
			}
		}
	}
}

bool UAruFunctionLibrary::ModifyAssets(
	const TArray<UObject*>& AssetsToModify,
	const TArray<FAruActionDefinition>& Actions,
	const FAruProcessConfig& Configs)
{
	// Initialize all proxy instances
	InitializeActionProxies(Actions);

	return ProcessAssets(AssetsToModify, Configs,
		[&Actions, &Configs](const FProperty* InPropertyPtr, void* InValuePtr)
			{
				bool bSuccess = false;
				for (const auto& Action : Actions)
				{
					bSuccess |= Action.Invoke(InPropertyPtr, InValuePtr, Configs.Parameters);
				}
				return bSuccess;
			});
}


bool UAruFunctionLibrary::ModifySelectedAssets(const TArray<FAruActionDefinition>& Actions, const FAruProcessConfig& Configs)
{
	const TArray<UObject*>&& SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	return ModifyAssets(SelectedObjects, Actions, Configs);
}

bool UAruFunctionLibrary::ValidateAssets(
	const TArray<UObject*>& AssetsToValidate,
	const TArray<FAruValidationDefinition>& Validations,
	const FAruProcessConfig& Configs)
{
	// Initialize all proxy instances
	InitializeValidationProxies(Validations);

	return ProcessAssets(AssetsToValidate, Configs,
		[&Validations, &Configs](const FProperty* InPropertyPtr, const void* InValuePtr)
			{
				for (const auto& Validation : Validations)
				{
					// Applies each validation rule to the current property
					// Uses "fail-fast" approach: if any validation fails, immediately returns false
					// without checking the remaining validation rules for this property
					if (!Validation.Validate(InPropertyPtr, InValuePtr, Configs.Parameters))
					{
						return false;
					}
				}
				// If all validations pass for this property, return true
				return true;
			});
}

bool UAruFunctionLibrary::ValidateSelectedAssets(const TArray<FAruValidationDefinition>& Validations, const FAruProcessConfig& Configs)
{
	const TArray<UObject*>&& SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	return ValidateAssets(SelectedObjects, Validations, Configs);
}

bool UAruFunctionLibrary::ProcessAssets(
	const TArray<UObject*>& Objects,
	const FAruProcessConfig& Configs,
	const TFunction<bool(const FProperty*, void*)>& PropertyProcessor)
{
	FScopedSlowTask Progress(Objects.Num(), LOCTEXT("Processing...", "Processing..."));
	Progress.MakeDialog();

	bool Result = false;
	for (auto& Object : Objects)
	{
		Progress.EnterProgressFrame(1.f);
		Result |= ProcessAsset(Object, Configs, PropertyProcessor);
	}

	return Result;
}

bool UAruFunctionLibrary::ProcessAsset(
	UObject* const Object,
	const FAruProcessConfig& Configs,
	const TFunction<bool(const FProperty*, void*)>& PropertyProcessor)
{
	UObject* ObjectToProcess = Object;
	UClass* ClassToProcess = Object->GetClass();
	if (UBlueprint* BlueprintAsset = Cast<UBlueprint>(Object))
	{
		ClassToProcess = BlueprintAsset->GeneratedClass;
		ObjectToProcess = ClassToProcess->GetDefaultObject();
	}

	bool bExecutedSuccessfully = false;
	for (TFieldIterator<FProperty> It{ClassToProcess}; It; ++It)
	{
		FProperty* Property = *It;
		if (Property == nullptr)
		{
			continue;
		}
		void* ValuePtr = Property->ContainerPtrToValuePtr<void>(ObjectToProcess);
		if (ValuePtr == nullptr)
		{
			continue;
		}

		bExecutedSuccessfully |= ProcessContainerValues(Property, ValuePtr, {PropertyProcessor, Configs.Parameters, Configs.MaxSearchDepth});
	}

	if (bExecutedSuccessfully)
	{
		Object->Modify();
	}

	return bExecutedSuccessfully;
}

bool UAruFunctionLibrary::ProcessContainerValues(
	FProperty* PropertyPtr,
	void* ValuePtr,
	const FAruProcessingParameters& InParameters)
{
	if (InParameters.RemainTime <= 0 || PropertyPtr == nullptr || ValuePtr == nullptr)
	{
		return false;
	}

	bool bExecutedSuccessfully = false;
	if (FObjectPropertyBase* ObjectProperty = CastField<FObjectPropertyBase>(PropertyPtr))
	{
		[&]
		{
			UObject* NativeObject = ObjectProperty->GetObjectPropertyValue(ValuePtr);
			if (NativeObject == nullptr)
			{
				return;
			}

			UClass* NativeClass = NativeObject->GetClass();
			if (NativeClass == nullptr)
			{
				return;
			}

			if (UBlueprint* BlueprintAsset = Cast<UBlueprint>(NativeObject))
			{
				NativeClass = BlueprintAsset->GeneratedClass;
				NativeObject = NativeClass->GetDefaultObject();
			}

			for (TFieldIterator<FProperty> It{NativeClass}; It; ++It)
			{
				FProperty* Property = *It;
				if (Property == nullptr)
				{
					continue;
				}
				void* ObjectValuePtr = Property->ContainerPtrToValuePtr<void>(NativeObject);
				if (ValuePtr == nullptr)
				{
					continue;
				}
				bExecutedSuccessfully |= ProcessContainerValues(Property, ObjectValuePtr, InParameters.GetSubsequentParameters());
			}
		}();
	}
	else if (FStructProperty* StructProperty = CastField<FStructProperty>(PropertyPtr))
	{
		[&]
		{
			const UScriptStruct* StructType = StructProperty->Struct;
			if (StructType == nullptr)
			{
				return;
			}

			if (StructType == FGameplayTag::StaticStruct()
				|| StructType == FGameplayTagQuery::StaticStruct()
				|| StructType == FGameplayTagContainer::StaticStruct())
			{
				return;
			}

			if (StructType == FInstancedStruct::StaticStruct())
			{
				FInstancedStruct* InstancedStructPtr = static_cast<FInstancedStruct*>(ValuePtr);
				if (InstancedStructPtr == nullptr || !InstancedStructPtr->IsValid())
				{
					return;
				}
				const UScriptStruct* InstancedStructType = InstancedStructPtr->GetScriptStruct();
				if (InstancedStructType == nullptr)
				{
					return;
				}
				void* InstancedStructContainer = InstancedStructPtr->GetMutableMemory();
				if (InstancedStructContainer == nullptr)
				{
					return;
				}

				for (TFieldIterator<FProperty> It{InstancedStructType}; It; ++It)
				{
					FProperty* Property = *It;
					if (Property == nullptr)
					{
						continue;
					}
					void* StructValuePtr = Property->ContainerPtrToValuePtr<void>(InstancedStructContainer);
					if (ValuePtr == nullptr)
					{
						continue;
					}
					bExecutedSuccessfully |= ProcessContainerValues(Property, StructValuePtr, InParameters.GetSubsequentParameters());
				}
			}
			else
			{
				for (TFieldIterator<FProperty> It{StructType}; It; ++It)
				{
					FProperty* Property = *It;
					if (Property == nullptr)
					{
						continue;
					}
					void* StructValuePtr = Property->ContainerPtrToValuePtr<void>(ValuePtr);
					if (ValuePtr == nullptr)
					{
						continue;
					}
					bExecutedSuccessfully |= ProcessContainerValues(Property, StructValuePtr, InParameters.GetSubsequentParameters());
				}
			}
		}();
	}
	else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(PropertyPtr))
	{
		FScriptArrayHelper ArrayHelper{ArrayProperty, ValuePtr};
		for (int32 Index = 0; Index < ArrayHelper.Num(); ++Index)
		{
			void* ItemPtr = ArrayHelper.GetRawPtr(Index);
			bExecutedSuccessfully |= ProcessContainerValues(ArrayProperty->Inner, ItemPtr, InParameters.GetSubsequentParameters());
		}
	}
	else if (FMapProperty* MapProperty = CastField<FMapProperty>(PropertyPtr))
	{
		FScriptMapHelper MapHelper{MapProperty, ValuePtr};
		for (int32 Index = 0; Index < MapHelper.Num(); ++Index)
		{
			void* MapKeyPtr = MapHelper.GetKeyPtr(Index);
			void* MapValuePtr = MapHelper.GetValuePtr(Index);
			bExecutedSuccessfully |= ProcessContainerValues(MapProperty->KeyProp, MapKeyPtr, InParameters.GetSubsequentParameters());
			bExecutedSuccessfully |= ProcessContainerValues(MapProperty->ValueProp, MapValuePtr, InParameters.GetSubsequentParameters());
		}
	}
	else if (FSetProperty* SetProperty = CastField<FSetProperty>(PropertyPtr))
	{
		FScriptSetHelper SetHelper{SetProperty, ValuePtr};
		for (int32 Index = 0; Index < SetHelper.Num(); ++Index)
		{
			void* ItemPtr = SetHelper.GetElementPtr(Index);
			bExecutedSuccessfully |= ProcessContainerValues(SetProperty->ElementProp, ItemPtr, InParameters.GetSubsequentParameters());
		}
	}

	bExecutedSuccessfully |= InParameters.PropertyProcessor(PropertyPtr, ValuePtr);

	return bExecutedSuccessfully;
}

FString UAruFunctionLibrary::ResolveParameterizedString(const FInstancedPropertyBag& InParameters, const FString& SourceString)
{
	if (SourceString.IsEmpty())
	{
		return SourceString;
	}

	const int32 OpenBraceIndex = SourceString.Find(TEXT("{"), ESearchCase::CaseSensitive, ESearchDir::FromStart);
	if(OpenBraceIndex == INDEX_NONE)
	{
		return SourceString;
	}

	const int32 CloseBraceIndex = SourceString.Find(TEXT("}"), ESearchCase::CaseSensitive, ESearchDir::FromStart, OpenBraceIndex + 1);
	if(CloseBraceIndex == INDEX_NONE)
	{
		return SourceString;
	}

	const FString Key = SourceString.Mid(OpenBraceIndex+1, CloseBraceIndex - OpenBraceIndex -1);
	TValueOrError<FString, EPropertyBagResult> SearchStringResult = InParameters.GetValueString(FName{Key});
	if (SearchStringResult.HasValue() == false)
	{
		return SourceString;
	}

	const FString Result = SourceString.Left(OpenBraceIndex) + SearchStringResult.GetValue() + SourceString.Right(SourceString.Len() - CloseBraceIndex - 1);
	return ResolveParameterizedString(InParameters, Result);
}

FAruPropertyContext UAruFunctionLibrary::FindPropertyByPath(
	const FProperty* InProperty,
	const void* InPropertyValue,
	const FString& Path)
{
	if (InProperty == nullptr || InPropertyValue == nullptr || Path.IsEmpty())
	{
		return {};
	}

	TArray<FString> PropertyChain;
	Path.ParseIntoArray(PropertyChain, TEXT("."), true);

	return FindPropertyByChain(InProperty, InPropertyValue, PropertyChain);
}

FAruPropertyContext UAruFunctionLibrary::FindPropertyByPath(
	const UStruct* InStructType,
	const void* InStructValue,
	const FString& Path)
{
	if (InStructType == nullptr || InStructValue == nullptr || Path.IsEmpty())
	{
		return {};
	}

	TArray<FString> PropertyChain;
	Path.ParseIntoArray(PropertyChain, TEXT("."), true);
	if (PropertyChain.Num() == 0)
	{
		return {};
	}
	
	const FProperty* FirstProperty = FindPropertyByName(InStructType, *PropertyChain[0]);
	if (FirstProperty == nullptr)
	{
		return {};
	}

	const void* FirstPropertyValue = FirstProperty->ContainerPtrToValuePtr<void>(InStructValue);
	if (FirstPropertyValue == nullptr)
	{
		return {};
	}

	TArrayView<FString> RemainPropertyChain = TArrayView<FString>{PropertyChain}.RightChop(1);
	return FindPropertyByChain(FirstProperty, FirstPropertyValue, RemainPropertyChain);
}

const FProperty* UAruFunctionLibrary::FindPropertyByName(const UStruct* InStruct, const FString& DisplayName)
{
	if (!InStruct || DisplayName.IsEmpty())
	{
		return nullptr;
	}

	for (FProperty* Property = InStruct->PropertyLink; Property != nullptr; Property = Property->PropertyLinkNext)
	{
		// First try to match the raw internal name (e.g., for C++ properties or simple cases)
		// This can directly match properties like TMap and TArray without special handling
		FString InternalName = Property->GetName();
		if (InternalName == DisplayName)
		{
			return Property;
		}

		// Try to get display name from property display text
		FText PropDisplayName = Property->GetDisplayNameText();
		if (!PropDisplayName.IsEmpty() && PropDisplayName.ToString() == DisplayName)
		{
			return Property;
		}

		// If the property has a metadata tag for display name, check that
		if (Property->HasMetaData(TEXT("DisplayName")))
		{
			FString MetaDisplayName = Property->GetMetaData(TEXT("DisplayName"));
			if (!MetaDisplayName.IsEmpty() && MetaDisplayName == DisplayName)
			{
				return Property;
			}
		}

		// Handle common UE naming conventions by removing auto-generated suffixes
		// This can handle properties including arrays and maps without special type handling
		FString CleanName = InternalName;
		int32 UnderscoreIndex;
		if (CleanName.FindChar('_', UnderscoreIndex) && UnderscoreIndex > 0)
		{
			// Check if there is a digit after the underscore, indicating an auto-generated suffix by UE
			if (UnderscoreIndex + 1 < CleanName.Len() && FChar::IsDigit(CleanName[UnderscoreIndex + 1]))
			{
				CleanName = CleanName.Left(UnderscoreIndex);
				if (CleanName == DisplayName)
				{
					return Property;
				}
			}
		}
	}
	return nullptr;
}

FAruPropertyContext UAruFunctionLibrary::FindPropertyByChain(
	const FProperty* InProperty,
	const void* InPropertyValue,
	const TArrayView<FString> PropertyChain)
{
	const FProperty* CurrentProperty = InProperty;
	const void* CurrentPropertyValue = InPropertyValue;

	// If there are no more properties in the chain, return the current property
    if (PropertyChain.Num() == 0)
    {
        return FAruPropertyContext{ const_cast<FProperty*>(CurrentProperty), const_cast<void*>(CurrentPropertyValue) };
    }

	for (const FString& Element : PropertyChain)
	{
		// Make sure we have proper setup to go through the process.
		if (CurrentProperty == nullptr || CurrentPropertyValue == nullptr)
		{
			return {};
		}

		if (const FObjectPropertyBase* ObjectProperty = CastField<FObjectPropertyBase>(CurrentProperty))
		{
			UObject* ObjectPtr = ObjectProperty->GetObjectPropertyValue(CurrentPropertyValue);
			if (ObjectPtr == nullptr)
			{
				return {};
			}

			const UClass* ClassType = ObjectPtr->GetClass();
			if (ClassType == nullptr)
			{
				return {};
			}

			CurrentProperty = FindPropertyByName(ClassType, Element);
			if (CurrentProperty == nullptr)
			{
				// TODO: Add Log.
				return {};
			}

			// Ignore nullptr check. If the value is nullptr, we will return nullptr.
			CurrentPropertyValue = CurrentProperty->ContainerPtrToValuePtr<void>(ObjectPtr);
		}
		else if (const FStructProperty* StructProperty = CastField<FStructProperty>(CurrentProperty))
		{
			const UScriptStruct* StructType = StructProperty->Struct;
			if (StructType == nullptr)
			{
				return {};
			}

			if (StructType == FInstancedStruct::StaticStruct())
			{
				const FInstancedStruct* InstancedStructPtr = static_cast<const FInstancedStruct*>(CurrentPropertyValue);
				if (InstancedStructPtr == nullptr || !InstancedStructPtr->IsValid())
				{
					return {};
				}

				const UScriptStruct* InstancedStructType = InstancedStructPtr->GetScriptStruct();
				if (InstancedStructType == nullptr)
				{
					return {};
				}

				CurrentProperty = FindPropertyByName(InstancedStructType, Element);
				if (CurrentProperty == nullptr)
				{
					// TODO: Add Log.
					return {};
				}

				const void* InstancedStructContainer = InstancedStructPtr->GetMemory();
				if (InstancedStructContainer == nullptr)
				{
					return {};
				}

				CurrentPropertyValue = CurrentProperty->ContainerPtrToValuePtr<void>(InstancedStructContainer);
				if (CurrentPropertyValue == nullptr)
				{
					return {};
				}
			}
			else
			{
				CurrentProperty = FindPropertyByName(StructType, Element);
				if (CurrentProperty == nullptr)
				{
					// TODO: Add Log.
					return {};
				}

				CurrentPropertyValue = CurrentProperty->ContainerPtrToValuePtr<void>(CurrentPropertyValue);
				if (CurrentPropertyValue == nullptr)
				{
					return {};
				}
			}
		}
		else 
		{
			return {};
		}
		// This check is not needed because we should continue processing the next property for each property handled.
		// TODO: Remove this check in the next version.
		// if (PropertyChain.Num() > 0 && (&Element == &PropertyChain.Last()))
		// {
		// 	return FAruPropertyContext{ const_cast<FProperty*>(CurrentProperty), const_cast<void*>(CurrentPropertyValue) };
		// }
	}

	return FAruPropertyContext{ const_cast<FProperty*>(CurrentProperty), const_cast<void*>(CurrentPropertyValue) }; return FAruPropertyContext{ const_cast<FProperty*>(CurrentProperty), const_cast<void*>(CurrentPropertyValue) };
}

bool UAruFunctionLibrary::IsTagMatching(const UAruAssetObject* AssetObject, const TArray<FName>& Tags)
{
	// If asset object is null, we cannot match tags
	if (AssetObject == nullptr)
	{
		return false;
	}

	// If Definition tag list is empty, consider it a match (no tag restrictions)
	if (Tags.IsEmpty())
	{
		return true;
	}

	// If AssetObject has no tags but Definition requires tags, no match
	if (AssetObject->GetAssetTags().IsEmpty())
	{
		return false;
	}

	// Check if object's tags contain ALL tags from the definition
	for (const FName& DefinitionTag : Tags)
	{
		if (!AssetObject->GetAssetTags().Contains(DefinitionTag))
		{
			// If any definition tag is missing from the object tags, no match
			return false;
		}
	}

	// All definition tags were found in the object tags
	return true;
}

#undef LOCTEXT_NAMESPACE
