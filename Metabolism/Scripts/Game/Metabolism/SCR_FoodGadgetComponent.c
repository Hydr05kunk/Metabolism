class SCR_FoodGadgetComponentClass : SCR_GadgetComponentClass
{
}

class SCR_FoodGadgetComponent : SCR_GadgetComponent
{
    [Attribute(defvalue: "50", desc: "Amount of energy added per use.")]
    protected int m_iEnergyValue;

    [Attribute(defvalue: "5", desc: "Cooldown in seconds between food uses.")]
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
            Print("Food gadget activated.");
            ApplyFoodEffect(); // Apply food logic
        }
        else
        {
            Print("Food gadget deactivated.");
        }
    }

    override void ActivateAction()
    {
        if (m_CharacterOwner && m_iMode == EGadgetMode.IN_HAND) // Ensure gadget is in hand
        {
            ToggleActive(!m_bActivated, SCR_EUseContext.FROM_ACTION);
        }
    }

    protected void ApplyFoodEffect()
    {
        Print("Food applied.");

        // Try to get the owner entity and check if it's the character or its parent
        IEntity owner = GetOwner();
        IEntity characterOwner = owner;
        if (!MetabolismComponent.Cast(owner.FindComponent(MetabolismComponent)))
        {
            characterOwner = owner.GetParent();  // Check the parent entity
        }
        
        if (characterOwner)
        {
            MetabolismComponent metabolismComponent = MetabolismComponent.Cast(characterOwner.FindComponent(MetabolismComponent));
            if (metabolismComponent)
            {
                Print("MetabolismComponent found.");
                metabolismComponent.IncreaseEnergy(m_iEnergyValue);
                float currentEnergy = metabolismComponent.GetEnergy();
                Print("Increased energy in MetabolismComponent by " + m_iEnergyValue + ". Current Energy: " + currentEnergy);
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
        Rpc(RpcShowFoodHint, "Hunger Satisfied");

        // Delete the gadget after use
        Delete();
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    protected void RpcShowFoodHint(string message)
    {
        SCR_HintManagerComponent.ShowCustomHint(message);
    }

    void Delete()
    {
        // Implement the logic to delete the gadget
        if (m_CharacterOwner)
        {
            Print("Deleting food gadget.");
            
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
            Print("Food gadget equipped.");
        }
        else if (mode == EGadgetMode.IN_STORAGE)
        {
            Print("Food gadget stowed.");
        }
    }

    int GetEnergyValue()
    {
        return m_iEnergyValue;
    }

    float GetCooldownTime()
    {
        return m_fCooldownTime;
    }
}
