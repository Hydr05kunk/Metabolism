class HydratePlayerAction : ScriptedUserAction
{
    [Attribute(defvalue: "20.0", desc: "Hydration added per use. Can be negative for dehydration.")]
    protected float m_iWaterValue;

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        Print("Initiating hydration action...");

        // Ensure the user performing the action is the owner
        if (!EL_NetworkUtils.IsOwner(pOwnerEntity))
        {
            Print("Action not performed by the owner.");
            return;
        }

        // Apply hydration effect to the player
        ApplyHydrationEffect(pUserEntity);
    }

    protected void ApplyHydrationEffect(IEntity characterOwner)
    {
        if (characterOwner)
        {
            MetabolismComponent metabolismComponent = MetabolismComponent.Cast(characterOwner.FindComponent(MetabolismComponent));
            if (metabolismComponent)
            {
                Print("MetabolismComponent found.");

                // Apply hydration changes (can be negative, e.g., for thirst increase)
                metabolismComponent.IncreaseHydration(m_iWaterValue);

                float currentHydration = metabolismComponent.GetHydration();
                Print("Increased hydration by " + m_iWaterValue);
                Print("Current Hydration: " + currentHydration);
            }
            else
            {
                Print("MetabolismComponent not found on character.");
            }
        }
        else
        {
            Print("Character not found.");
        }
    }
	override void OnActionStart(IEntity pUserEntity)
	{
		ChimeraCharacter character = ChimeraCharacter.Cast(pUserEntity);
		if (!character)
			return;

		CharacterControllerComponent charController = character.GetCharacterController();
		if (charController)
		{
			CharacterAnimationComponent pAnimationComponent = charController.GetAnimationComponent();
			int itemActionId = pAnimationComponent.BindCommand("CMD_Drink");
			pAnimationComponent.CallCommand(itemActionId,0,0);
		}
	}
}
