require "UnLua"

local DefaultDamage = Class()

function DefaultDamage:K2_PreApplyToComponent(DamageEvent)
    print("DefaultDamage:PreApplyToComponent from " .. DamageEvent.DamageCauser:GetName())
end

return DefaultDamage
