#include "AssetFilters/AruFilter_Proxy.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AruFilter_Proxy)
void FAruFilter_BlueprintProxy::InitializeProxy(UObject* InOwner)
{
    // Check if there is already a valid ProxyInstance
    if (ProxyInstance != nullptr)
    {
        // If the current instance's type does not match ProxyClass, a new instance needs to be created
        if (ProxyClass && !ProxyInstance->IsA(ProxyClass))
        {
            ProxyInstance = nullptr;
        }
        else
        {
            // Instance already exists and is of the correct type, no action needed
            return;
        }
    }

    // If ProxyClass is not set, an instance cannot be created
    if (!ProxyClass)
    {
        return;
    }

    // Use the provided Owner or default to GetTransientPackage()
    UObject* Owner = InOwner ? InOwner : GetTransientPackage();
    
    // Create a new ProxyInstance
    ProxyInstance = NewObject<UAruFilterProxy>(Owner, ProxyClass);
}

// Ensure the proxy instance is properly initialized before use
bool FAruFilter_BlueprintProxy::EnsureProxyInitialized() const
{
    if (!ProxyInstance || (ProxyClass && !ProxyInstance->IsA(ProxyClass)))
    {
        // The proxy instance needs to be initialized
        FAruFilter_BlueprintProxy* MutableThis = const_cast<FAruFilter_BlueprintProxy*>(this);
        if (MutableThis)
        {
            MutableThis->InitializeProxy();
        }
    }
    
    return ProxyInstance != nullptr;
}

bool FAruFilter_Proxy::IsConditionMet(
	const FProperty* InProperty,
	const void* InValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (InProperty == nullptr)
	{
		return false;
	}

	if(InValue == nullptr)
	{
		return false;
	}

	if(const FBoolProperty* BoolProperty = CastField<FBoolProperty>(InProperty))
	{
		return IsConditionMet_Internal(BoolProperty->GetPropertyValue(InValue), InParameters);
	}

	if(const FIntProperty* IntProperty = CastField<FIntProperty>(InProperty))
	{
		return IsConditionMet_Internal(IntProperty->GetPropertyValue(InValue), InParameters);
	}

	if(const FFloatProperty* FloatProperty = CastField<FFloatProperty>(InProperty))
	{
		return IsConditionMet_Internal(FloatProperty->GetPropertyValue(InValue), InParameters);
	}

	if(const FStrProperty* StrProperty = CastField<FStrProperty>(InProperty))
	{
		return IsConditionMet_Internal(StrProperty->GetPropertyValue(InValue), InParameters);
	}

	if(const FTextProperty* TextProperty = CastField<FTextProperty>(InProperty))
	{
		return IsConditionMet_Internal(TextProperty->GetPropertyValue(InValue), InParameters);
	}

	if(const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(InProperty))
	{
		return IsConditionMet_Internal(ObjectProperty->GetObjectPropertyValue(InValue), InParameters);
	}

	if(const FSoftObjectProperty* SoftObjectProperty = CastField<FSoftObjectProperty>(InProperty))
	{
		return IsConditionMet_Internal(SoftObjectProperty->GetObjectPropertyValue(InValue), InParameters);
	}

	if(const FNameProperty* NameProperty = CastField<FNameProperty>(InProperty))
	{
		return IsConditionMet_Internal(NameProperty->GetPropertyValue(InValue), InParameters);
	}

	if(const FEnumProperty* EnumProperty = CastField<FEnumProperty>(InProperty))
	{
		return IsConditionMet_Internal(EnumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue(InValue), EnumProperty->GetEnum(), InParameters);
	}

	if (const FStructProperty* StructProperty = CastField<FStructProperty>(InProperty))
	{
		const UScriptStruct* StructType = StructProperty->Struct;
		if(StructType == nullptr)
		{
			return false;
		}

		if(StructType == FGameplayTag::StaticStruct())
		{
			return IsConditionMet_Internal(*static_cast<const FGameplayTag*>(InValue), InParameters);
		}

		if(StructType == FGameplayTagContainer::StaticStruct())
		{
			return IsConditionMet_Internal(*static_cast<const FGameplayTagContainer*>(InValue), InParameters);
		}

		if(StructType == FInstancedStruct::StaticStruct())
		{
			return IsConditionMet_Internal(*static_cast<const FInstancedStruct*>(InValue), InParameters);
		}

		return IsConditionMet_Internal(FConstStructView{StructType, static_cast<const uint8*>(InValue)}, InParameters);
	}
	
	return false;
}

bool UAruFilterProxy::CheckBoolValue_Implementation(
	const bool InBoolValue) const
{
	return false;
}

bool UAruFilterProxy::CheckIntValue_Implementation(
	const int32 InIntValue) const
{
	return false;
}

bool UAruFilterProxy::CheckFloatValue_Implementation(
	const float InFloatValue) const
{
	return false;
}

bool UAruFilterProxy::CheckStringValue_Implementation(
	const FString& InStringValue) const
{
	return false;
}

bool UAruFilterProxy::CheckTextValue_Implementation(
	const FText& InTextValue) const
{
	return false;
}

bool UAruFilterProxy::CheckObjectValue_Implementation(
	const UObject* InObjectValue) const
{
	return false;
}

bool UAruFilterProxy::CheckNameValue_Implementation(
	const FName& InProperty) const
{
	return false;
}

// bool UAruFilterProxy::CheckStructValue_Implementation(
// 	const FInstancedStruct& InStruct) const
// {
// 	return false;
// }

bool UAruFilterProxy::CheckInstancedStructValue_Implementation(
	const FInstancedStruct& InInstancedStruct) const
{
	return false;
}

bool UAruFilterProxy::CheckGameplayTagValue_Implementation(
	const FGameplayTag& InGameplayTag) const
{
	return false;
}

bool UAruFilterProxy::CheckGameplayTagContainerValue_Implementation(
	const FGameplayTagContainer& InGameplayTagContainer) const
{
	return false;
}

bool UAruFilterProxy::CheckEnumValue_Implementation(
	const int32 InEnumValue,
	const UEnum* InEnumType) const
{
	return false;
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const bool InBoolValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckBoolValue(InBoolValue);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const int32 InIntValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckIntValue(InIntValue);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const float InFloatValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckFloatValue(InFloatValue);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const FString& InStringValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckStringValue(InStringValue);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const FText& InTextValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckTextValue(InTextValue);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const UObject* InObjectValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckObjectValue(InObjectValue);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const FName& InProperty,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckNameValue(InProperty);
}



bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const FInstancedStruct& InInstancedStruct,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckInstancedStructValue(InInstancedStruct);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const FGameplayTag& InGameplayTag,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckGameplayTagValue(InGameplayTag);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const FGameplayTagContainer& InGameplayTagContainer,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckGameplayTagContainerValue(InGameplayTagContainer);
}

bool FAruFilter_BlueprintProxy::IsConditionMet_Internal(
	const int32 InEnumValue,
	const UEnum* InEnumType,
	const FInstancedPropertyBag& InParameters) const
{
	if (!EnsureProxyInitialized())
	{
		return false;
	}

	return ProxyInstance->CheckEnumValue(InEnumValue, InEnumType);
}
