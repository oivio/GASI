#pragma once

#include "Abilities/GameplayAbility.h"

#include "GASIGameplayAbility.generated.h"

class UGASIAbilitySystemComponent;

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;

/**
 * AbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputReleased);

UCLASS()
class GASI_API UGASIGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "GASI|Ability")
	void OnInputPressed();

	UFUNCTION(BlueprintImplementableEvent, Category = "GASI|Ability")
	void OnInputReleased();

	UFUNCTION(BlueprintCallable, Category = "GASI|Ability")
	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:

	//~UGameplayAbility interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UGameplayAbility interface

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Ability")
	EAbilityActivationPolicy ActivationPolicy;

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

};