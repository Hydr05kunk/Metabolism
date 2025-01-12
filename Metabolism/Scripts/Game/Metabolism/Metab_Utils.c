class Metab_Utils
{
    // Apply damage to the character using a direct health modification
    static void ApplyDamage(IEntity entity, float damage)
    {
        if (!entity)
            return;

        // Get the damage manager component or health component
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(entity.FindComponent(DamageManagerComponent));
        if (damageManager)
        {
            // Access the default hit zone
            HitZone defaultHitZone = damageManager.GetDefaultHitZone();
            if (defaultHitZone)
            {
                // Calculate new health after applying damage
                float currentHealth = defaultHitZone.GetHealth();
                float newHealth = currentHealth - damage;

                // Ensure health doesn't drop below zero
                newHealth = Math.Max(newHealth, 0);

                // Update the hit zone's health
                defaultHitZone.SetHealth(newHealth);
            }
        }
    }
}
