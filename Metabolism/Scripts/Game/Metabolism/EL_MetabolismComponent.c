class MetabolismComponentClass : ScriptComponentClass
{
    // Define any metadata or additional properties/methods for MetabolismComponentClass here
    // This is a placeholder class for now
}

class MetabolismComponent : ScriptComponent
{
    // Attributes related to metabolism
    private float m_fHydration = 0.55;        // Hydration level (0.0 to 1.0), capped at 0.55
    private float m_fEnergy = 0.55;           // Energy level (0.0 to 1.0), capped at 0.55

    private float m_fDehydrationDamage = 1.0; // Damage per second from dehydration
    private float m_fStarvationDamage = 1.0;  // Damage per second from starvation

    private bool m_bIsThirsty = false;        // Indicates if the character is thirsty
    private bool m_bIsHungry = false;         // Indicates if the character is hungry

    // New sickness-related attributes
    private float m_fSicknessLevel = 0.0;     // Sickness level (0.0 to 1.0)
    private float m_fSicknessIncreaseRate = 0.01; // Rate at which sickness increases
    private float m_fRecoveryRate = 0.005;    // Rate at which character recovers from sickness
    private bool m_bIsSick = false;           // Indicates if the character is sick

    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);

        // Set the update interval to 5 seconds (5000 milliseconds)
        GetGame().GetCallqueue().CallLater(UpdateMetabolism, 5000, true, 1); // 5-second interval, time slice of 1
    }

    void UpdateMetabolism(float timeSlice)
    {
        // Drain hydration and energy, apply damage if necessary
        HandleHydration(timeSlice);
        HandleEnergy(timeSlice);

        // Update sickness
        UpdateSickness(timeSlice);
    }

    void HandleHydration(float timeSlice)
    {
        if (m_fHydration > 0)
        {
            m_fHydration = Math.Clamp(m_fHydration - 0.004, 0.0, 0.55);
        }
        else
        {
            ApplyDamageFromDehydration(timeSlice);
        }
        m_bIsThirsty = m_fHydration < 0.1;
    }

    void HandleEnergy(float timeSlice)
    {
        if (m_fEnergy > 0)
        {
            m_fEnergy = Math.Clamp(m_fEnergy - 0.002, 0.0, 0.55);
        }
        else
        {
            ApplyDamageFromStarvation(timeSlice);
        }
        m_bIsHungry = m_fEnergy < 0.1;
    }

    void UpdateSickness(float timeSlice)
    {
        if (m_bIsSick)
        {
            // Increase sickness level over time
            m_fSicknessLevel = Math.Clamp(m_fSicknessLevel + m_fSicknessIncreaseRate * timeSlice, 0.0, 1.0);

            // Apply damage due to sickness
            Metab_Utils.ApplyDamage(GetOwner(), m_fSicknessIncreaseRate * timeSlice);
        }
        else if (m_fSicknessLevel > 0)
        {
            // Decrease sickness level over time (recovery)
            m_fSicknessLevel = Math.Clamp(m_fSicknessLevel - m_fRecoveryRate * timeSlice, 0.0, 1.0);
        }

        // Check if the character becomes sick or recovers
        m_bIsSick = m_fSicknessLevel > 0.2;
    }

    void ApplyDamageFromDehydration(float timeSlice)
    {
        if (m_fHydration <= 0)
        {
            // Apply damage over time due to dehydration
            Metab_Utils.ApplyDamage(GetOwner(), m_fDehydrationDamage * timeSlice);
        }
    }

    void ApplyDamageFromStarvation(float timeSlice)
    {
        if (m_fEnergy <= 0)
        {
            // Apply damage over time due to starvation
            Metab_Utils.ApplyDamage(GetOwner(), m_fStarvationDamage * timeSlice);
        }
    }

    bool CanSprint()
    {
        return m_fHydration >= 0.1 && !m_bIsSick; // Sprinting is allowed only if hydration is above 10% and not sick
    }

    void IncreaseHydration(float amount)
    {
        m_fHydration = Math.Clamp(m_fHydration + amount, 0.0, 0.55);
    }

    void IncreaseEnergy(float amount)
    {
        m_fEnergy = Math.Clamp(m_fEnergy + amount, 0.0, 0.55);
    }

    void IncreaseSickness(float amount)
    {
        m_fSicknessLevel = Math.Clamp(m_fSicknessLevel + amount, 0.0, 1.0);
    }

    float GetHydration() { return m_fHydration; }
    float GetEnergy() { return m_fEnergy; }
    bool IsThirsty() { return m_bIsThirsty; }
    bool IsHungry() { return m_bIsHungry; }
    bool IsSick() { return m_bIsSick; }
    float GetSicknessLevel() { return m_fSicknessLevel; }
}
