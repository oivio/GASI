#include "AbilitySystem/GASIGameAbilityDataAsset.h"

UGASIGameAbilityDataAsset::UGASIGameAbilityDataAsset(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

const TSet<FGameplayInputAbilityInfo>& UGASIGameAbilityDataAsset::GetInputAbilities() const
{
    return InputAbilities;
}

#if WITH_EDITOR
    void UGASIGameAbilityDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
    {
        Super::PostEditChangeProperty(PropertyChangedEvent);

        const FProperty* Property = PropertyChangedEvent.Property;
        if (Property && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGASIGameAbilityDataAsset, InputAbilities) && !InputAbilities.IsEmpty())
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