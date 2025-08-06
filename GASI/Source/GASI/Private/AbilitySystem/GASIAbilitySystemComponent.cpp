// Copyright Epic Games, Inc. All Rights Reserved.
#include "AbilitySystem/GASIAbilitySystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void UGASIAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC)) return;

	GrantDefaultAbilitiesAndAttributes(PC);

}

void UGASIAbilitySystemComponent::GrantDefaultAbilitiesAndAttributes(APlayerController* PC)
{
	if (AbilityActorInfo)
	{

		// Reset/Remove abilities if we had already added them
		{
			for (FGameplayAbilitySpecHandle AbilityHandle : GrantedAbilityHandles)
			{
				SetRemoveAbilityOnEnd(AbilityHandle);
			}

			GrantedAbilityHandles.Empty(GrantedAbilities.Num());
		}

		// Granted abilities
		{
			GrantedAbilityHandles.Reserve(GrantedAbilities.Num());
			for (const TSubclassOf<UGameplayAbility>& Ability : GrantedAbilities)
			{
				if (*Ability)
				{
					GrantedAbilityHandles.Add(GiveAbility(FGameplayAbilitySpec(Ability)));
				}
			}

			if (!InputAbilities.IsEmpty())
			{

				if (!IsValid(AbilityActorInfo->OwnerActor.Get())) return;
				if (!PC && !PC->GetLocalPlayer()) return;
				
				UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
				if (!Subsystem) return;

				if (!AbilityMappingContext)
					return;
				else
					Subsystem->AddMappingContext(AbilityMappingContext, 0);

				UEnhancedInputComponent* InputComponent = CastChecked<UEnhancedInputComponent>(PC->InputComponent);
				if (!InputComponent) return;

				for (const auto& It : InputAbilities)
				{
					if (It.IsValid() && InputComponent)
					{

						const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(It.GameplayAbilityClass, 1, It.InputID);

						GrantedAbilityHandles.Add(GiveAbility(FGameplayAbilitySpec(AbilitySpec)));
						GiveAbility(AbilitySpec);

						const UInputAction* InputAction = It.InputAction;
						const int32 InputID = It.InputID;

						InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UGASIAbilitySystemComponent::OnAbilityInputPressed, InputID);
						InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UGASIAbilitySystemComponent::OnAbilityInputReleased, InputID);

					}
				}
			}
		}

		// Granted Gameplay Effects
		{

			for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
			{
				if (!GameplayEffect) continue;

				FGameplayEffectContextHandle EffectContext = MakeEffectContext();
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(GameplayEffect, 1.0f, EffectContext);
				if (SpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}

	}

}

void UGASIAbilitySystemComponent::OnAbilityInputPressed(int32 InputID)
{

	// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
	//InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	AbilityLocalInputPressed(InputID);
		
}

void UGASIAbilitySystemComponent::OnAbilityInputReleased(int32 InputID)
{

	// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
	//InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	AbilityLocalInputReleased(InputID);
		
}

// To make sure that there will be no more than 1 instance of a pair of the same GameplayAbilityClass AND the same InputAction.
#if WITH_EDITOR
	void UGASIAbilitySystemComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		const FProperty* Property = PropertyChangedEvent.Property;
		if (Property && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGASIAbilitySystemComponent, InputAbilities) && !InputAbilities.IsEmpty())
		{
			TArray<FGameplayInputAbilityInfo> InputAbilitiesArray = InputAbilities.Array();

			for (int32 i = InputAbilitiesArray.Num() - 1; i > 0; --i)
			{
				const int32 PrevIndex = i - 1;
				if (InputAbilitiesArray[i] == InputAbilitiesArray[PrevIndex])
				{
					InputAbilitiesArray.RemoveAtSwap(i);
				}
			}

			InputAbilities.Reset();

			for (int32 i = 0; i < InputAbilitiesArray.Num(); ++i)
			{
				FGameplayInputAbilityInfo& Info = InputAbilitiesArray[i];
				Info.InputID = i;
				InputAbilities.Add(Info);
			}
		}
	}
#endif