#pragma once
#include "AruTypes.h"
#include "AruFilter_Proxy.generated.h"

USTRUCT(meta=(Hidden))
struct FAruFilter_Proxy : public FAruFilter
{
	GENERATED_BODY()
public:
	virtual ~FAruFilter_Proxy() override {}
	virtual bool IsConditionMet(const FProperty* InProperty, const void* InValue, const FInstancedPropertyBag& InParameters) const override;
protected:
	virtual bool IsConditionMet_Internal(const bool InBoolValue, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const int32 InIntValue, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const float InFloatValue, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const FString& InStringValue, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const FText& InTextValue, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const UObject* InObjectValue, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const FName& InProperty, const FInstancedPropertyBag& InParameters) const {return false;}

	virtual bool IsConditionMet_Internal(const FConstStructView InStruct, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const FInstancedStruct& InInstancedStruct, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const FGameplayTag& InGameplayTag, const FInstancedPropertyBag& InParameters) const {return false;}
	virtual bool IsConditionMet_Internal(const FGameplayTagContainer& InGameplayTagContainer, const FInstancedPropertyBag& InParameters) const {return false;}

	virtual bool IsConditionMet_Internal(const int32 InEnumValue, const UEnum* InEnumType, const FInstancedPropertyBag& InParameters) const {return false;}
};

UCLASS(BlueprintType, Blueprintable)
class ARUEDITORUTILITIES_API UAruFilterProxy : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckBoolValue(const bool InBoolValue) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckIntValue(const int32 InIntValue) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckFloatValue(const float InFloatValue) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckStringValue(const FString& InStringValue) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckTextValue(const FText& InTextValue) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckObjectValue(const UObject* InObjectValue) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckNameValue(const FName& InProperty) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckInstancedStructValue(const FInstancedStruct& InInstancedStruct) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckGameplayTagValue(const FGameplayTag& InGameplayTag) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckGameplayTagContainerValue(const FGameplayTagContainer& InGameplayTagContainer) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AruFilterProxy")
	bool CheckEnumValue(const int32 InEnumValue, const UEnum* InEnumType) const;
};

USTRUCT(BlueprintType, DisplayName="Blueprint Proxy")
struct FAruFilter_BlueprintProxy : public FAruFilter_Proxy
{
	GENERATED_BODY()
public:
	virtual ~FAruFilter_BlueprintProxy() override {}

	// Initialize the ProxyInstance based on ProxyClass
	void InitializeProxy(UObject* InOwner = nullptr);

	// Helper function to ensure ProxyInstance is initialized before use
	bool EnsureProxyInitialized() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAruFilterProxy> ProxyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UAruFilterProxy> ProxyInstance;

	virtual bool IsConditionMet_Internal(const bool InBoolValue, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const int32 InIntValue, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const float InFloatValue, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const FString& InStringValue, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const FText& InTextValue, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const UObject* InObjectValue, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const FName& InProperty, const FInstancedPropertyBag& InParameters) const override;

	// virtual bool IsConditionMet_Internal(const FConstStructView InStruct, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const FInstancedStruct& InInstancedStruct, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const FGameplayTag& InGameplayTag, const FInstancedPropertyBag& InParameters) const override;
	virtual bool IsConditionMet_Internal(const FGameplayTagContainer& InGameplayTagContainer, const FInstancedPropertyBag& InParameters) const override;

	virtual bool IsConditionMet_Internal(const int32 InEnumValue, const UEnum* InEnumType, const FInstancedPropertyBag& InParameters) const override;
};
