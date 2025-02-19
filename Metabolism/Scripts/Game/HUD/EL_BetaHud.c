class EL_BetaHud : SCR_InfoDisplay
{
    //--------------------------- CONFIG VALUES ---------------------------
    protected bool m_bEnableHUD = true;
    protected bool m_bEnableStamina = true;
    protected bool m_bEnableHealth = true;
    protected bool m_bEnableThirst = true;
    protected bool m_bEnableHunger = true;
    protected bool m_bEnableMoney = true;
    protected bool m_bEnableSickness = true;
    //---------------------------------------------------------------------

    protected HorizontalLayoutWidget m_wPlayerStatsHUD;

    protected ProgressBarWidget m_wStaminaProgressBar;
    protected ImageWidget m_wStaminaProgress;
    protected ImageWidget m_wHealthProgress;
    protected ImageWidget m_wThirstProgress;
    protected ImageWidget m_wHungerProgress;
    protected ImageWidget m_wSickProgress;

    protected TextWidget m_wMoneyIndicator;
    protected OverlayWidget m_wHealthIndicator;
    protected OverlayWidget m_wStaminaIndicator;
    protected OverlayWidget m_wHungerIndicator;
    protected OverlayWidget m_wThirstIndicator;
    protected OverlayWidget m_wSickIndicator;

    protected SCR_CharacterControllerComponent m_PlayerController;
    protected DamageManagerComponent m_DMC;

    protected bool m_bUsingProgressWidget;
    protected bool m_bStatChange;
    protected float m_fPreviousValues[5];

    protected float m_fTimeAccumulator;
    protected bool m_bGUIHidden;

    protected MetabolismComponent m_metabolismComponent;

    void SetMetabolismComponent(MetabolismComponent metabolismComponent)
    {
        m_metabolismComponent = metabolismComponent;
    }

    void OnHealthChange(float value)
    {
        if (!m_wHealthProgress)
        {
            m_wHealthProgress = ImageWidget.Cast(m_wHealthIndicator.FindAnyWidget("m_healthProgress"));
            if (!m_wHealthProgress) return;
        }
        m_wHealthProgress.SetMaskProgress(value);
        SetProgressColor(m_wHealthProgress, value);
    }

    void OnStaminaChange(float value)
    {
        if (!m_wStaminaProgress)
        {
            m_wStaminaProgress = ImageWidget.Cast(m_wStaminaIndicator.FindAnyWidget("m_staminaProgress"));
            if (!m_wStaminaProgress) return;
        }
        m_wStaminaProgress.SetMaskProgress(value);
        SetProgressColor(m_wStaminaProgress, value);
    }

    void OnThirstChange(float value)
    {
        if (!m_wThirstProgress)
        {
            m_wThirstProgress = ImageWidget.Cast(m_wThirstIndicator.FindAnyWidget("m_thirstProgress"));
            if (!m_wThirstProgress) return;
        }
        m_wThirstProgress.SetMaskProgress(value);
        SetProgressColor(m_wThirstProgress, value);
    }

    void OnHungerChange(float value)
    {
        if (!m_wHungerProgress)
        {
            m_wHungerProgress = ImageWidget.Cast(m_wHungerIndicator.FindAnyWidget("m_hungerProgress"));
            if (!m_wHungerProgress) return;
        }
        m_wHungerProgress.SetMaskProgress(value);
        SetProgressColor(m_wHungerProgress, value);
    }

    void OnMoneyChange(ResourceName prefab, int newAmount, int oldAmount)
    {
        if (!m_wMoneyIndicator)
        {
            m_wMoneyIndicator = TextWidget.Cast(m_wPlayerStatsHUD.FindAnyWidget("m_moneyIndicator"));
            if (!m_wMoneyIndicator) return;
        }
        m_wMoneyIndicator.SetText("$ " + newAmount);
    }

    void OnSicknessChange(float value)
    {
        if (!m_wSickProgress)
        {
            m_wSickProgress = ImageWidget.Cast(m_wSickIndicator.FindAnyWidget("m_sickProgress"));
            if (!m_wSickProgress) return;
        }
        m_wSickProgress.SetMaskProgress(Math.Clamp(value, 0.0, 1.0));
        SetProgressColor(m_wSickProgress, value);
    }

    override event void UpdateValues(IEntity owner, float timeSlice)
    {
        if (!m_PlayerController) return;

        m_bStatChange = false;

        if (m_bEnableThirst && m_metabolismComponent)
        {
            OnThirstChange(m_metabolismComponent.GetHydration() / 1.0);
        }
        if (m_bEnableHunger && m_metabolismComponent)
        {
            OnHungerChange(m_metabolismComponent.GetEnergy() / 1.0);
        }
        if (m_bEnableHealth) OnHealthChange(m_DMC.GetHealth());
        if (m_bEnableStamina) OnStaminaChange(m_PlayerController.GetStamina());
        if (m_bEnableSickness && m_metabolismComponent) OnSicknessChange(m_metabolismComponent.GetSicknessLevel());

        if (m_bStatChange)
        {
            m_fTimeAccumulator = 0;
            if (m_bGUIHidden)
            {
                m_bGUIHidden = false;
                ShowStatsHUD(true);
            }
        }
        else
        {
            m_fTimeAccumulator += timeSlice;
            if (m_fTimeAccumulator > 4.0 && !m_bGUIHidden)
            {
                m_bGUIHidden = true;
                ShowStatsHUD(false);
            }
        }
    }

    void ShowStatsHUD(bool show)
    {
        AnimateWidget.Opacity(m_wPlayerStatsHUD, show, 0.5);
    }

    void SetProgressColor(Widget bar, float value)
    {
        if (value >= 0.55) bar.SetColor(Color.White);
        else if (value > 0.275) bar.SetColor(Color.Yellow);
        else bar.SetColor(Color.Red);
    }
}
