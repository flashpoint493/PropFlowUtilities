#pragma once

#include "StructUtils/InstancedStruct.h"
#include "StructUtils/PropertyBag.h"
#include "AruTypes.generated.h"

namespace Aru::ProcessResult
{
	static FString Error{"ERROR"};
	static FString Failed{"FAILED"};
	static FString Success{"SUCCESS"};
}

UENUM(BlueprintType)
enum class EAruNumericCompareOp : uint8
{
	Equip,
	NotEqual,
	GreaterThan,
	LessThan
};

UENUM(BlueprintType)
enum class EAruBooleanCompareOp : uint8
{
	Is,
	Not
};

UENUM(BlueprintType)
enum class EAruContainerCompareOp : uint8
{
	HasAny,
	HasAll
};

UENUM(BlueprintType)
enum class EAruNameCompareOp : uint8
{
	MatchAny,
	MatchAll
};

USTRUCT(BlueprintType)
struct FAruFilter
{
	GENERATED_BODY()

public:
	virtual ~FAruFilter() {}

	/**
	 * Executes an operation on a target property using provided parameters.
	 * 
	 * @param InProperty        The meta-data/description of the property being operated on. 
	 *                          Contains type information and property attributes.
	 * @param InValue           Pointer to the raw memory address of the property instance 
	 *                          being modified. The actual data type matches InProperty's type.
	 * @param InParameters      Container holding runtime parameters/operators for the operation,
	 *                          including any global context.
	 * 
	 * @return                  True if the property value met the condition, false otherwise.
	 */
	virtual bool IsConditionMet(const FProperty* InProperty, const void* InValue, const FInstancedPropertyBag& InParameters) const { return bInverseCondition; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Config, meta=(AdvancedClassDisplay))
	bool bInverseCondition = false;
};

USTRUCT(BlueprintType)
struct FAruPredicate
{
	GENERATED_BODY()

public:
	virtual ~FAruPredicate() {};

	/**
	 * Executes an operation on a target property using provided parameters.
	 * 
	 * @param InProperty        The meta-data/description of the property being operated on. 
	 *                          Contains type information and property attributes.
	 * @param InValue           Pointer to the raw memory address of the property instance 
	 *                          being modified. The actual data type matches InProperty's type.
	 * @param InParameters      Container holding runtime parameters/operators for the operation,
	 *                          including any global context.
	 * 
	 * @return                  True if the property value was modified during execution,
	 *                          false if no changes were made or the operation failed.
	 */
	virtual bool Execute(const FProperty* InProperty, void* InValue, const FInstancedPropertyBag& InParameters) const { return true; }
};

template <typename StructType>
	struct TStructIterator
{
	int32 Index;
	const TArray<TInstancedStruct<StructType>>& StructArrayRef;

	FORCEINLINE TStructIterator(int32 InIndex, const TArray<TInstancedStruct<StructType>>& InStructArray)
		: Index(InIndex), StructArrayRef(InStructArray) {};

	FORCEINLINE bool operator!=(const TStructIterator& Other) const { return Index != Other.Index; }
	FORCEINLINE const StructType& operator*() const { return StructArrayRef[Index].template Get<const StructType>(); }
	FORCEINLINE TStructIterator& operator++()
	{
		++Index;
		return *this;
	}
};

template <typename StructType>
struct TRangedForStructArray
{
	const TArray<TInstancedStruct<StructType>>& StructArrayRef;

	explicit TRangedForStructArray(const TArray<TInstancedStruct<StructType>>& InStructArray) : StructArrayRef(InStructArray) {}

	FORCEINLINE TStructIterator<StructType> begin() { return TStructIterator<StructType>(0, StructArrayRef); }
	FORCEINLINE TStructIterator<StructType> end()	{ return TStructIterator<StructType>(StructArrayRef.Num(), StructArrayRef); }
};

USTRUCT(BlueprintType)
struct FAruActionDefinition
{
	GENERATED_BODY()

public:
	bool Invoke(const FProperty* InProperty, void* InValue, const FInstancedPropertyBag& InParameters) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aru Editor Utilities", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FAruFilter>> ActionConditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aru Editor Utilities", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FAruPredicate>> ActionPredicates;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aru Editor Utilities", meta=(DisplayName="Description"))
	FString Description;
	
	/** Tags for matching with asset objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aru Editor Utilities", meta=(DisplayName="Tags"))
	TArray<FName> ActionTags;
	
public:
	FORCEINLINE TRangedForStructArray<FAruFilter> ForEachCondition() const
	{
		return TRangedForStructArray{ActionConditions};
	}

	FORCEINLINE TRangedForStructArray<FAruPredicate> ForEachPredicates() const
	{
		return TRangedForStructArray{ActionPredicates};
	}

	/** Get all action tags */
	FORCEINLINE const TArray<FName>& GetTags() const
	{
		return ActionTags;
	}
};

USTRUCT(BlueprintType)
struct FAruValidationDefinition
{
	GENERATED_BODY()

public:
	bool Validate(const FProperty* InProperty, const void* InValue, const FInstancedPropertyBag& InParameters) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aru Editor Utilities", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FAruFilter>> ValidationConditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aru Editor Utilities", meta = (DisplayName = "Description"))
	FString Description;

	/** Tags for matching with asset objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aru Editor Utilities", meta=(DisplayName="Tags"))
	TArray<FName> ValidationTags;

public:
	FORCEINLINE TRangedForStructArray<FAruFilter> ForEachCondition() const
	{
		return TRangedForStructArray{ValidationConditions};
	}

	/** Get all validation tags */
	FORCEINLINE const TArray<FName>& GetTags() const
	{
		return ValidationTags;
	}
};

UCLASS(BlueprintType)
class ARUEDITORUTILITIES_API UAruActionConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAruActionDefinition> ActionDefinitions;
};

UCLASS(BlueprintType)
class ARUEDITORUTILITIES_API UAruValidationConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAruValidationDefinition> ValidationDefinitions;
};

USTRUCT(BlueprintType)
struct FAruProcessConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FInstancedPropertyBag Parameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSearchDepth = 5;
};
