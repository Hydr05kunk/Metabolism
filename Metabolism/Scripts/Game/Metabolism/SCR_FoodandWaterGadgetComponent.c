class SCR_FoodWaterGadgetComponentClass : SCR_GadgetComponentClass
{
}

class SCR_FoodWaterGadgetComponent : SCR_GadgetComponent
{
    [Attribute(defvalue: "50.0", desc: "Energy added per use. Can be negative for hunger loss.")]
    protected float m_iEnergyValue;

    [Attribute(defvalue: "20.0", desc: "Hydration added per use. Can be negative for dehydration.")]
    protected float m_iWaterValue;

    [Attribute(defvalue: "10.0", desc: "Amount of sickness added.")]
    protected float m_fSicknessValue; // This controls the sickness amount directly.

    override EGadgetType GetType()
    {
        return EGadgetType.CONSUMABLE;
    }

    override void OnToggleActive(bool state)
    {
        m_bActivated = state;

        if (m_bActivated)
        {
            ApplyMetabolismEffect(); // Apply food, water, and sickness logic
        }
    }

    override void ActivateAction()
    {
        if (m_CharacterOwner && m_iMode == EGadgetMode.IN_HAND)
        {
            ToggleActive(!m_bActivated, SCR_EUseContext.FROM_ACTION);
        }
    }

    protected void ApplyMetabolismEffect()
    {
        IEntity owner = GetOwner();
        IEntity characterOwner = owner;
        if (!MetabolismComponent.Cast(owner.FindComponent(MetabolismComponent)))
        {
            characterOwner = owner.GetParent();
        }
        
        if (characterOwner)
        {
            MetabolismComponent metabolismComponent = MetabolismComponent.Cast(characterOwner.FindComponent(MetabolismComponent));
            if (metabolismComponent)
            {
                metabolismComponent.IncreaseEnergy(m_iEnergyValue);
                metabolismComponent.IncreaseHydration(m_iWaterValue);

                // Directly apply the sickness effect
                metabolismComponent.IncreaseSickness(m_fSicknessValue);
            }
        }

        Delete();
    }

    float GetEnergyValue()
    {
        return m_iEnergyValue;
    }

    float GetWaterValue()
    {
        return m_iWaterValue;
    }

    float GetSicknessValue()
    {
        return m_fSicknessValue;
    }

    void Delete()
    {
        if (m_CharacterOwner)
        {
            SCR_EntityHelper.DeleteEntityAndChildren(GetOwner());
            m_bActivated = false;
        }
    }
}
