#include "AssetPredicates/AruPredicate_Proxy.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AruPredicate_Proxy)
void FAruPredicate_BlueprintProxy::InitializeProxy(UObject* InOwner)
{
    // Check if there is already a valid ProxyInstance
    if (ProxyInstance != nullptr)
    {
        // If the current instance's type does not match ProxyClass, then a new instance needs to be created
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
    ProxyInstance = NewObject<UAruPredicateProxy>(Owner, ProxyClass);
}

bool FAruPredicate_Proxy::Execute(
	const FProperty* InProperty,
	void* InValue,
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
		BoolProperty->SetPropertyValue(InValue, ExecuteBoolValue(BoolProperty->GetPropertyValue(InValue), InParameters));
		return true;
	}

	if(const FIntProperty* IntProperty = CastField<FIntProperty>(InProperty))
	{
		IntProperty->SetPropertyValue(InValue, ExecuteIntegerValue(IntProperty->GetPropertyValue(InValue), InParameters));
		return true;
	}
	
	if(const FFloatProperty* FloatProperty = CastField<FFloatProperty>(InProperty))
	{
		FloatProperty->SetPropertyValue(InValue, ExecuteFloatValue(FloatProperty->GetPropertyValue(InValue), InParameters));
		return true;
	}

	if(const FStrProperty* StringProperty = CastField<FStrProperty>(InProperty))
	{
		StringProperty->SetPropertyValue(InValue, ExecuteStringValue(StringProperty->GetPropertyValue_InContainer(InValue), InParameters));
		return true;
	}

	if(const FTextProperty* TextProperty = CastField<FTextProperty>(InProperty))
	{
		TextProperty->SetPropertyValue(InValue, ExecuteTextValue(TextProperty->GetPropertyValue_InContainer(InValue), InParameters));
		return true;
	}

	if(const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(InProperty))
	{
		ObjectProperty->SetObjectPropertyValue(InValue, ExecuteObjectValue(ObjectProperty->GetObjectPropertyValue_InContainer(InValue), InParameters));
		return true;
	}

	if(const FNameProperty* NameProperty = CastField<FNameProperty>(InProperty))
	{
		NameProperty->SetPropertyValue(InValue, ExecuteNameValue(NameProperty->GetPropertyValue(InValue), InParameters));
		return true;
	}

	if(const FStructProperty* StructProperty = CastField<FStructProperty>(InProperty))
	{
		const UScriptStruct* StructType = StructProperty->Struct;
		if(StructType == nullptr)
		{
			return false;
		}

		if(StructType == FGameplayTag::StaticStruct())
		{
			FGameplayTag* GameplayTag = static_cast<FGameplayTag*>(InValue);
			*GameplayTag = ExecuteGameplayTagValue(*GameplayTag, InParameters);
			return true;
		}

		if(StructType == FGameplayTagContainer::StaticStruct())
		{
			FGameplayTagContainer* GameplayTagContainer = static_cast<FGameplayTagContainer*>(InValue);
			*GameplayTagContainer = ExecuteGameplayTagContainerValue(*GameplayTagContainer, InParameters);
			return true;
		}

		if(StructType == FInstancedStruct::StaticStruct())
		{
			FInstancedStruct* InstancedStruct = static_cast<FInstancedStruct*>(InValue);
			*InstancedStruct = ExecuteInstancedStructValue(*InstancedStruct, InParameters);
			return true;
		}

		FInstancedStruct StructProxy;
		StructProxy.InitializeAs(StructType, static_cast<uint8*>(InValue));
		StructProperty->CopyCompleteValue(InValue, ExecuteStructValue(StructProxy, InParameters).GetMutableMemory());
		return true;
	}

	if(const FEnumProperty* EnumProperty = CastField<FEnumProperty>(InProperty))
	{
		if(const FNumericProperty* NumericProperty = CastField<FNumericProperty>(EnumProperty->GetUnderlyingProperty()))
		{
			NumericProperty->SetIntPropertyValue(InValue, ExecuteEnumValue(NumericProperty->GetSignedIntPropertyValue(InValue), EnumProperty->GetEnum(), InParameters));
			return true;
		}
		
		return false;
	}

	return false;
}

bool FAruPredicate_BlueprintProxy::ExecuteBoolValue(
	const bool InBoolValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessBoolValue(InBoolValue);
	}
	
	return InBoolValue;
}

int32 FAruPredicate_BlueprintProxy::ExecuteIntegerValue(
	const int32 InIntValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessIntValue(InIntValue);
	}
	
	return InIntValue;
}

float FAruPredicate_BlueprintProxy::ExecuteFloatValue(
	const float InFloatValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessFloatValue(InFloatValue);
	}
	
	return InFloatValue;
}

FString FAruPredicate_BlueprintProxy::ExecuteStringValue(
	const FString& InStringValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessStringValue(InStringValue);
	}

	return InStringValue;
}

FText FAruPredicate_BlueprintProxy::ExecuteTextValue(
	const FText& InTextValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessTextValue(InTextValue);
	}

	return InTextValue;
}

UObject* FAruPredicate_BlueprintProxy::ExecuteObjectValue(
	const UObject* InObjectValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessObjectValue(InObjectValue);
	}

	return const_cast<UObject*>(InObjectValue);
}

FName FAruPredicate_BlueprintProxy::ExecuteNameValue(
	const FName& InNameValue,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessNameValue(InNameValue);
	}

	return InNameValue;
}

FInstancedStruct FAruPredicate_BlueprintProxy::ExecuteStructValue(
	const FInstancedStruct& InStruct,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessStructValue(InStruct);
	}
	
	return InStruct;
}

FInstancedStruct FAruPredicate_BlueprintProxy::ExecuteInstancedStructValue(
	const FInstancedStruct& InInstancedStruct,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessInstancedStructValue(InInstancedStruct);
	}

	return InInstancedStruct;
}

FGameplayTag FAruPredicate_BlueprintProxy::ExecuteGameplayTagValue(
	const FGameplayTag& InGameplayTag,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessGameplayTagValue(InGameplayTag);
	}

	return InGameplayTag;
}

FGameplayTagContainer FAruPredicate_BlueprintProxy::ExecuteGameplayTagContainerValue(
	const FGameplayTagContainer& InGameplayTagContainer,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessGameplayTagContainerValue(InGameplayTagContainer);
	}

	return InGameplayTagContainer;
}

int64 FAruPredicate_BlueprintProxy::ExecuteEnumValue(
	const int64 InEnumValue,
	const UEnum* InEnumType,
	const FInstancedPropertyBag& InParameters) const
{
	if (ProxyInstance)
	{
		return ProxyInstance->ProcessEnumValue(InEnumValue, InEnumType);
	}

	return InEnumValue;
}
