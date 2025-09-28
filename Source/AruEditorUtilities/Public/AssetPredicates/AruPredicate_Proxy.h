#pragma once
#include "AruTypes.h"
#include "AruPredicate_Proxy.generated.h"

USTRUCT(meta=(Hidden))
struct FAruPredicate_Proxy : public FAruPredicate
{
	GENERATED_BODY()
public:
	virtual ~FAruPredicate_Proxy() override {}
	virtual bool Execute(const FProperty* InProperty, void* InValue, const FInstancedPropertyBag& InParameters) const override;
protected:
	virtual bool ExecuteBoolValue(const bool InBoolValue, const FInstancedPropertyBag& InParameters) const {return InBoolValue;}
	virtual int32 ExecuteIntegerValue(const int32 InIntValue, const FInstancedPropertyBag& InParameters) const {return InIntValue;}
	virtual float ExecuteFloatValue(const float InFloatValue, const FInstancedPropertyBag& InParameters) const {return InFloatValue;}
	virtual FString ExecuteStringValue(const FString& InStringValue, const FInstancedPropertyBag& InParameters) const {return InStringValue;}
	virtual FText ExecuteTextValue(const FText& InTextValue, const FInstancedPropertyBag& InParameters) const {return InTextValue;}
	virtual UObject* ExecuteObjectValue(const UObject* InObjectValue, const FInstancedPropertyBag& InParameters) const {return nullptr;}
	virtual FName ExecuteNameValue(const FName& InNameValue, const FInstancedPropertyBag& InParameters) const {return InNameValue;}

	virtual FInstancedStruct ExecuteStructValue(const FInstancedStruct& InStruct, const FInstancedPropertyBag& InParameters) const {return InStruct;}
	virtual FInstancedStruct ExecuteInstancedStructValue(const FInstancedStruct& InInstancedStruct, const FInstancedPropertyBag& InParameters) const {return InInstancedStruct;}
	virtual FGameplayTag ExecuteGameplayTagValue(const FGameplayTag& InGameplayTag, const FInstancedPropertyBag& InParameters) const {return InGameplayTag;}
	virtual FGameplayTagContainer ExecuteGameplayTagContainerValue(const FGameplayTagContainer& InGameplayTagContainer, const FInstancedPropertyBag& InParameters) const {return InGameplayTagContainer;}

	virtual int64 ExecuteEnumValue(const int64 InEnumValue, const UEnum* InEnumType, const FInstancedPropertyBag& InParameters) const {return InEnumValue;}
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class ARUEDITORUTILITIES_API UAruPredicateProxy : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	bool ProcessBoolValue(const bool InBoolValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	int32 ProcessIntValue(const int32 InIntValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	float ProcessFloatValue(const float InFloatValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FString ProcessStringValue(const FString& InStringValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FText ProcessTextValue(const FText& InTextValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	UObject* ProcessObjectValue(const UObject* InObjectValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FName ProcessNameValue(const FName& InNameValue) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FInstancedStruct ProcessStructValue(const FInstancedStruct& InStruct) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FInstancedStruct ProcessInstancedStructValue(const FInstancedStruct& InInstancedStruct) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FGameplayTag ProcessGameplayTagValue(const FGameplayTag& InGameplayTag) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	FGameplayTagContainer ProcessGameplayTagContainerValue(const FGameplayTagContainer& InGameplayTagContainer) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="AruPredicateProxy")
	int64 ProcessEnumValue(const int64 InEnumValue, const UEnum* InEnumType) const;
};

USTRUCT(BlueprintType, DisplayName="Blueprint Proxy")
struct FAruPredicate_BlueprintProxy : public FAruPredicate_Proxy
{
	GENERATED_BODY()
public:
	virtual ~FAruPredicate_BlueprintProxy() override {}

	// Initialize the ProxyInstance based on ProxyClass
	void InitializeProxy(UObject* InOwner = nullptr);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAruPredicateProxy> ProxyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UAruPredicateProxy> ProxyInstance;
	
	virtual bool ExecuteBoolValue(const bool InBoolValue, const FInstancedPropertyBag& InParameters) const override;
	virtual int32 ExecuteIntegerValue(const int32 InIntValue, const FInstancedPropertyBag& InParameters) const override;
	virtual float ExecuteFloatValue(const float InFloatValue, const FInstancedPropertyBag& InParameters) const override;
	virtual FString ExecuteStringValue(const FString& InStringValue, const FInstancedPropertyBag& InParameters) const override;
	virtual FText ExecuteTextValue(const FText& InTextValue, const FInstancedPropertyBag& InParameters) const override;
	virtual UObject* ExecuteObjectValue(const UObject* InObjectValue, const FInstancedPropertyBag& InParameters) const override;
	virtual FName ExecuteNameValue(const FName& InNameValue, const FInstancedPropertyBag& InParameters) const override;
	virtual FInstancedStruct ExecuteStructValue(const FInstancedStruct& InStruct, const FInstancedPropertyBag& InParameters) const override;
	virtual FInstancedStruct ExecuteInstancedStructValue(const FInstancedStruct& InInstancedStruct, const FInstancedPropertyBag& InParameters) const override;
	virtual FGameplayTag ExecuteGameplayTagValue(const FGameplayTag& InGameplayTag, const FInstancedPropertyBag& InParameters) const override;
	virtual FGameplayTagContainer ExecuteGameplayTagContainerValue(const FGameplayTagContainer& InGameplayTagContainer, const FInstancedPropertyBag& InParameters) const override;
	virtual int64 ExecuteEnumValue(const int64 InEnumValue, const UEnum* InEnumType, const FInstancedPropertyBag& InParameters) const override;
};
