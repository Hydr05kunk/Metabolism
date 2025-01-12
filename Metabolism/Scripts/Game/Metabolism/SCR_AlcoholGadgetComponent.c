class SCR_AlcoholGadgetComponentClass : SCR_GadgetComponentClass
{
}

class SCR_AlcoholGadgetComponent : SCR_GadgetComponent
{
    [Attribute(defvalue: "10", desc: "Amount of alcohol added per use.")]
    protected int m_iAlcoholValue;

    [Attribute(defvalue: "5", desc: "Cooldown in seconds between alcohol uses.")]
    protected float m_fCooldownTime;

    protected float m_fLastUseTime = -m_fCooldownTime;

    protected SCR_CharacterControllerComponent m_characterControllerComponent;
    protected SCR_CharacterDamageManagerComponent m_damageManager;

    override EGadgetType GetType()
    {
        return EGadgetType.CONSUMABLE;
    }

    override void OnToggleActive(bool state)
    {
        m_bActivated = state;

        if (m_bActivated)
        {
            Print("Alcohol gadget activated.");
            ApplyAlcoholEffect(); // Apply alcohol logic
        }
        else
        {
            Print("Alcohol gadget deactivated.");
        }
    }

    override void ActivateAction()
    {
        if (m_CharacterOwner && m_iMode == EGadgetMode.IN_HAND) // Ensure gadget is in hand
        {
            ToggleActive(!m_bActivated, SCR_EUseContext.FROM_ACTION);
        }
    }

    protected void ApplyAlcoholEffect()
    {
        Print("Alcohol applied.");

        IEntity owner = GetOwner();
        IEntity characterOwner = owner.GetParent(); // Ensure the owner is the character
        if (!characterOwner)
        {
            Print("Character not found.");
            return;
        }

        MetabolismComponent metabolismComponent = MetabolismComponent.Cast(characterOwner.FindComponent(MetabolismComponent));
        if (metabolismComponent)
        {
            Print("MetabolismComponent found on character.");
            metabolismComponent.IncreaseAlcoholLevel(m_iAlcoholValue);
            float currentAlcoholLevel = metabolismComponent.GetAlcoholLevel();
            Print("Increased alcohol level in MetabolismComponent by " + m_iAlcoholValue + ". Current Alcohol Level: " + currentAlcoholLevel);

            // Check if alcohol level hits 100 and set the permission for unconsciousness
            if (currentAlcoholLevel >= 100.0)
            {
                Print("Alcohol level is 100 or more. Setting unconsciousness permission.");
                m_characterControllerComponent = SCR_CharacterControllerComponent.Cast(characterOwner.FindComponent(SCR_CharacterControllerComponent));
                m_damageManager = SCR_CharacterDamageManagerComponent.Cast(characterOwner.FindComponent(SCR_CharacterDamageManagerComponent));
                if (m_characterControllerComponent && m_damageManager)
                {
                    SetUncon();
                }
                else
                {
                    Print("CharacterControllerComponent or CharacterDamageManagerComponent not found on character.");
                }
            }
        }
        else
        {
            Print("MetabolismComponent not found on character.");
        }

        // Show the hint message
        Rpc(RpcShowAlcoholHint, "Alcohol Level Increased");

        // Delete the gadget after use
        Delete();
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    protected void RpcShowAlcoholHint(string message)
    {
        SCR_HintManagerComponent.ShowCustomHint(message);
    }

    void Delete()
    {
        // Implement the logic to delete the gadget
        if (m_CharacterOwner)
        {
            Print("Deleting alcohol gadget.");
            
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
            Print("Alcohol gadget equipped.");
        }
        else if (mode == EGadgetMode.IN_STORAGE)
        {
            Print("Alcohol gadget stowed.");
        }
    }

    int GetAlcoholValue()
    {
        return m_iAlcoholValue;
    }

    float GetCooldownTime()
    {
        return m_fCooldownTime;
    }

    void SetUncon()
    {
        if (!m_characterControllerComponent.IsUnconscious())
        {
            m_damageManager.EnableDamageHandling(false);
            m_characterControllerComponent.SetUnconscious(true);
            
            GetGame().GetCallqueue().CallLater(SetUnconOff, 120000, false); // 120,000 milliseconds = 2 minutes
        }
    }
    
    void SetUnconOff()
    {
        if (m_characterControllerComponent.IsUnconscious())
        {
            m_characterControllerComponent.SetUnconscious(false);
            m_damageManager.EnableDamageHandling(true);
        }
    }
}
