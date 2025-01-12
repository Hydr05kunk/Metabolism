modded class SCR_CharacterControllerComponent : PrimaryControllerComponent
{
    // Define the Update method without the override keyword
    void Update(float timeSlice)
    {
        // Custom logic for managing hydration and sprinting
        MetabolismComponent metabolism = MetabolismComponent.Cast(GetOwner().FindComponent(MetabolismComponent));
        if (metabolism)
        {
            metabolism.UpdateMetabolism(timeSlice);

            // Allow sprinting only if the character is hydrated enough
            if (!metabolism.CanSprint())
            {
                // Stop sprinting if hydration is too low
                StopSprinting();
            }
        }
    }

    // Define the StopSprinting method
    void StopSprinting()
    {
        // Logic to stop sprinting
        Print("Sprinting stopped due to low hydration.");
        // Implement specific logic to stop sprinting, such as changing character state or animation
    }
}
