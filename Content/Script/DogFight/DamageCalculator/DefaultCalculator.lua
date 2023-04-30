local ArrayHelper = require "Common.ArrayHelper"
local DefaultCalculator = UnrealClass()

function DefaultCalculator:GetDamageInstanceList()
    local List = {
        "DefaultDamage",
    }

    return ArrayHelper.StringArrayFromTable(List)
end

return DefaultCalculator
