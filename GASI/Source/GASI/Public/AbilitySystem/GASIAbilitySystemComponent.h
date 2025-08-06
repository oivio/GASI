// MyProject/Source/MyProject/Public/MyAbilitySystemComponent.h

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GASIGameAbilityDataAsset.h"

#include "GASIAbilitySystemComponent.generated.h"

class AController;
class APlayerController;
class UInputMappingContext;

UCLASS(meta = (BlueprintSpawnableComponent))
class GASI_API UGASIAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** MappingContext for gameplay Input Abilities*/
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UInputMappingContext> AbilityMappingContext;

	/** Gameplay Abilities that will be trigger by Input*/
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TSet<FGameplayInputAbilityInfo> InputAbilities;

	/** Gameplay Abilities that will GrantedAbilities at BeginPlay*/
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UGASIAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void GrantDefaultAbilitiesAndAttributes(APlayerController* PC);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void BeginPlay() override;

	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

private:

	void OnAbilityInputPressed(int32 InputID);
	void OnAbilityInputReleased(int32 InputID);

};