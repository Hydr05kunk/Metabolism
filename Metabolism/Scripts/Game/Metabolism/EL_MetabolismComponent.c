class MetabolismComponentClass : ScriptComponentClass
{
    // Define any metadata or additional properties/methods for MetabolismComponentClass here
    // This is a placeholder class for now
}

class MetabolismComponent : ScriptComponent
{
    // Attributes related to metabolism
    private float m_fHydration = 50.0;        // Hydration level (0 to 50)
    private float m_fEnergy = 50.0;           // Energy level (0 to 50)
    private float m_fAlcoholLevel = 0.0;       // Alcohol level (0 to 100)
    
    private float m_fThirstRate = 2.0;         // Rate at which hydration decreases
    private float m_fHungerRate = 1.0;         // Rate at which energy decreases
    private float m_fDehydrationDamage = 1.0;  // Damage per second from dehydration
    private float m_fStarvationDamage = 1.0;   // Damage per second from starvation
    private float m_fAlcoholDecayRate = 1.0;   // Rate at which alcohol decreases

    private bool m_bIsThirsty = false;         // Indicates if the character is thirsty
    private bool m_bIsHungry = false;          // Indicates if the character is hungry

    private MetabolismHUD m_PlayerHUD;         // Reference to the HUD

    private SoundComponent soundComp;          // Sound component for playing sounds
    private AudioHandle m_AudioHandle = AudioHandle.Invalid;
    ref SCR_AudioSource audioSource1;
    protected ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
    private vector m_vSoundOffset;

    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        Print("MetabolismComponent initialized.");

        // Initialize sound component
        soundComp = SoundComponent.Cast(owner.FindComponent(SoundComponent));
        
        if (soundComp)
        {
            Print("Sound component initialized.");
            
            // Play a test sound to verify it works
            m_AudioHandle = soundComp.PlayStr("SOUND_FART");
            Print("Test sound played during initialization.");
        }
        else
        {
            Print("Sound component not found.");
        }

        GetGame().GetCallqueue().CallLater(UpdateMetabolism, 60000, true, 1); // 1-minute interval, time slice of 1
    }

    void SetHUD(MetabolismHUD hud)
    {
        m_PlayerHUD = hud;
        Print("HUD set in MetabolismComponent.");
    }

    void HandleConsumption(string type, float increaseAmount)
    {
        if (type == "Food")
        {
            // Increase energy when food is consumed
            m_fEnergy = Math.Clamp(m_fEnergy + increaseAmount, 0.0, 50.0);
            Print("Food consumed. Energy increased to " + m_fEnergy);

            // Check if energy reached 50 and play sound
            if (soundComp && m_fEnergy == 50)
            {
                Print("Energy reached 50. Playing sound...");
                m_AudioHandle = soundComp.PlayStr("SOUND_FART");
            }

            m_bIsHungry = false; // Character is no longer hungry after eating
        }
        else if (type == "Water")
        {
            // Increase hydration when water is consumed
            m_fHydration = Math.Clamp(m_fHydration + increaseAmount, 0.0, 50.0);
            m_bIsThirsty = false; // Character is no longer thirsty after drinking
            Print("Water consumed. Hydration increased to " + m_fHydration);
        }
        else if (type == "Alcohol")
        {
            // Increase alcohol level when alcohol is consumed
            m_fAlcoholLevel = Math.Clamp(m_fAlcoholLevel + increaseAmount, 0.0, 100.0);
            Print("Alcohol consumed. Alcohol level increased to " + m_fAlcoholLevel);
            CheckRagdollState();
        }
    }

    void CheckRagdollState()
    {
        if (m_fAlcoholLevel >= 75.0)
        {
            TriggerRagdoll();
        }
    }

    void TriggerRagdoll()
    {
        IEntity owner = GetOwner();
        if (owner)
        {
            Print("Ragdoll state triggered.");
        }
    }

    void UpdateMetabolism(float timeSlice)
    {
        Print("Updating metabolism...");
        
        // Reduce hydration over time
        if (m_fHydration > 0)
        {
            m_fHydration -= m_fThirstRate * timeSlice;
            Print("Hydration reduced: " + m_fHydration);
        }
        else
        {
            m_fHydration = 0;
            ApplyDamageFromDehydration(timeSlice);
        }

        // Reduce energy over time
        if (m_fEnergy > 0)
        {
            m_fEnergy -= m_fHungerRate * timeSlice;
            Print("Energy reduced: " + m_fEnergy);
        }
        else
        {
            m_fEnergy = 0;
            ApplyDamageFromStarvation(timeSlice);
        }

        // Decay alcohol level over time
        if (m_fAlcoholLevel > 0)
        {
            m_fAlcoholLevel = Math.Clamp(m_fAlcoholLevel - m_fAlcoholDecayRate * timeSlice, 0.0, 100.0);
            Print("Alcohol level reduced: " + m_fAlcoholLevel);
        }

        // Check if the character is thirsty or hungry
        m_bIsThirsty = m_fHydration < 10.0;
        m_bIsHungry = m_fEnergy < 10.0;

        // Update the HUD with the current metabolism values
        if (m_PlayerHUD)
        {
            Print("Updating HUD...");
            m_PlayerHUD.OnThirstChange(m_fHydration / 50.0);
            m_PlayerHUD.OnHungerChange(m_fEnergy / 50.0);
        }

        Print("Metabolism update completed. Hydration: " + m_fHydration + ", Energy: " + m_fEnergy + ", Alcohol Level: " + m_fAlcoholLevel);
    }

    void ApplyDamageFromDehydration(float timeSlice)
    {
        if (m_fHydration <= 0)
        {
            // Apply damage over time due to dehydration
            Metab_Utils.ApplyDamage(GetOwner(), m_fDehydrationDamage * timeSlice);
            Print("Dehydration damage applied.");
        }
    }

    void ApplyDamageFromStarvation(float timeSlice)
    {
        if (m_fEnergy <= 0)
        {
            // Apply damage over time due to starvation
            Metab_Utils.ApplyDamage(GetOwner(), m_fStarvationDamage * timeSlice);
            Print("Starvation damage applied.");
        }
    }

    // Handle sprinting when thirsty
    bool CanSprint()
    {
        return m_fHydration >= 10.0; // Sprinting is allowed only if hydration is above 10%
    }
    
    // Method to increase hydration
    void IncreaseHydration(float amount)
    {
        m_fHydration = Math.Clamp(m_fHydration + amount, 0.0, 50.0);
        Print("Hydration increased. Hydration: " + m_fHydration);
    }

    // Method to increase energy
    void IncreaseEnergy(float amount)
    {
        m_fEnergy = Math.Clamp(m_fEnergy + amount, 0.0, 50.0);
        Print("Energy increased. Energy: " + m_fEnergy);

        // Check if energy reached 50 and play sound
        if (soundComp && m_fEnergy == 50)
        {
            Print("Energy reached 50. Playing sound...");
            m_AudioHandle = soundComp.PlayStr("SOUND_FART");
        }
    }

    // Method to increase alcohol level
    void IncreaseAlcoholLevel(float amount)
    {
        m_fAlcoholLevel = Math.Clamp(m_fAlcoholLevel + amount, 0.0, 100.0);
        Print("Alcohol level increased. Alcohol Level: " + m_fAlcoholLevel);
    }

    // Getter methods for hydration, energy, and alcohol level
    float GetHydration() { return m_fHydration; }
    float GetEnergy() { return m_fEnergy; }
    float GetAlcoholLevel() { return m_fAlcoholLevel; }
    bool IsThirsty() { return m_bIsThirsty; }
    bool IsHungry() { return m_bIsHungry; }
}
