class SCR_HydrationGadgetComponentClass : SCR_GadgetComponentClass
{
}

class SCR_HydrationGadgetComponent : SCR_GadgetComponent
{
    [Attribute(defvalue: "50", desc: "Amount of hydration added per use.")]
    protected int m_iHydrationValue;

    [Attribute(defvalue: "5", desc: "Cooldown in seconds between hydration uses.")]
    protected float m_fCooldownTime;

    protected float m_fLastUseTime = -m_fCooldownTime;

    override EGadgetType GetType()
    {
        return EGadgetType.CONSUMABLE;
    }

    override void OnToggleActive(bool state)
    {
        m_bActivated = state;

        if (m_bActivated)
        {
            Print("Water gadget activated.");
            ApplyHydrationEffect(); // Apply hydration logic
        }
        else
        {
            Print("Water gadget deactivated.");
        }
    }

    override void ActivateAction()
    {
        if (m_CharacterOwner && m_iMode == EGadgetMode.IN_HAND) // Ensure gadget is in hand
        {
            ToggleActive(!m_bActivated, SCR_EUseContext.FROM_ACTION);
        }
    }

    protected void ApplyHydrationEffect()
    {
        Print("Hydration applied.");

        // Try to get the owner entity and check if it's the character or its parent
        IEntity owner = GetOwner();
        IEntity characterOwner = owner.GetParent();
        
        if (characterOwner)
        {
            MetabolismComponent metabolismComponent = MetabolismComponent.Cast(characterOwner.FindComponent(MetabolismComponent));
            if (metabolismComponent)
            {
                Print("MetabolismComponent found.");
                metabolismComponent.IncreaseHydration(m_iHydrationValue);
                float currentHydration = metabolismComponent.GetHydration();
                Print("Increased hydration in MetabolismComponent by " + m_iHydrationValue + ". Current Hydration: " + currentHydration);
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

        // Show the hint message
        Rpc(RpcShowHydrationHint, "Thirst Replenished");

        // Delete the gadget after use
        Delete();
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    protected void RpcShowHydrationHint(string message)
    {
        SCR_HintManagerComponent.ShowCustomHint(message);
    }

    void Delete()
    {
        // Implement the logic to delete the gadget
        if (m_CharacterOwner)
        {
            Print("Deleting hydration gadget.");
            
            // Delete the gadget entity and its children
            SCR_EntityHelper.DeleteEntityAndChildren(GetOwner());
            Print("Gadget entity and its children deleted.");
            
            m_bActivated = false;
        }
    }

    override void ModeSwitch(EGadgetMode mode, IEntity charOwner)
    {
        super.ModeSwitch(mode, charOwner);

        if (mode == EGadgetMode.IN_HAND)
        {
            Print("Hydration gadget equipped.");
        }
        else if (mode == EGadgetMode.IN_STORAGE)
        {
            Print("Hydration gadget stowed.");
        }
    }

    int GetHydrationValue()
    {
        return m_iHydrationValue;
    }

    float GetCooldownTime()
    {
        return m_fCooldownTime;
    }
}
