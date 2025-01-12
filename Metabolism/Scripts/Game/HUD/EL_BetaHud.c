class MetabolismHUD : SCR_InfoDisplay
{
    //--------------------------- CONFIG VALUES ---------------------------
    protected bool m_bEnableHUD = true;
    protected bool m_bEnableThirst = true;
    protected bool m_bEnableHunger = true;
    //---------------------------------------------------------------------

    protected HorizontalLayoutWidget m_wPlayerStatsHUD;

    protected ImageWidget m_wThirstProgress;
    protected ImageWidget m_wHungerProgress;

    protected OverlayWidget m_wHungerIndicator;
    protected OverlayWidget m_wThirstIndicator;

    protected SCR_CharacterControllerComponent m_PlayerController;

    protected bool m_bUsingProgressWidget;
    protected bool m_bStatChange;
    protected float m_fPreviousValues[2];

    // Reference to the MetabolismComponent
    protected MetabolismComponent m_metabolismComponent;

    // Setter for MetabolismComponent
    void SetMetabolismComponent(MetabolismComponent metabolismComponent)
    {
        m_metabolismComponent = metabolismComponent;
    }

    //---------------------------------------------------------------------
    void OnThirstChange(float value)
    {
        if (!m_wThirstProgress)
        {
            m_wThirstProgress = ImageWidget.Cast(m_wThirstIndicator.FindAnyWidget("m_thirstProgress"));
            if (!m_wThirstProgress) return;
        }

        m_wThirstProgress.SetMaskProgress(value);
    }

    //---------------------------------------------------------------------
    void OnHungerChange(float value)
    {
        if (!m_wHungerProgress)
        {
            m_wHungerProgress = ImageWidget.Cast(m_wHungerIndicator.FindAnyWidget("m_hungerProgress"));
            if (!m_wHungerProgress) return;
        }

        m_wHungerProgress.SetMaskProgress(value);
    }

    //---------------------------------------------------------------------
    override event void OnStartDraw(IEntity owner)
    {
        super.OnStartDraw(owner);

        if (!m_bEnableHUD) return;

        m_PlayerController = SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent));
        m_wPlayerStatsHUD = HorizontalLayoutWidget.Cast(m_wRoot.FindAnyWidget("m_playerStatsHUD"));

        if (!m_wPlayerStatsHUD) return;

        if (m_bEnableHunger)
        {
            m_wHungerIndicator = OverlayWidget.Cast(m_wPlayerStatsHUD.FindAnyWidget("m_hungerIndicator"));
            if (!m_wHungerIndicator) return;
        }

        if (m_bEnableThirst)
        {
            m_wThirstIndicator = OverlayWidget.Cast(m_wPlayerStatsHUD.FindAnyWidget("m_thirstIndicator"));
            if (!m_wThirstIndicator) return;
        }

        MetabolismComponent metabolismComponent = MetabolismComponent.Cast(owner.FindComponent(MetabolismComponent));
        if (metabolismComponent)
        {
            SetMetabolismComponent(metabolismComponent);
        }
    }

    //---------------------------------------------------------------------
    override event void UpdateValues(IEntity owner, float timeSlice)
    {
        if (!m_PlayerController) return;

        m_bStatChange = false;

        if (m_bEnableThirst && m_metabolismComponent)
        {
            OnThirstChange(m_metabolismComponent.GetHydration() / 100.0);
        }
        if (m_bEnableHunger && m_metabolismComponent)
        {
            OnHungerChange(m_metabolismComponent.GetEnergy() / 100.0);
        }
    }
}
