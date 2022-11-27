require "UnLua"

local ArrayHelper = require "Common.ArrayHelper"
local DefaultCalculator = Class()

function DefaultCalculator:GetDamageInstanceList()
    local List = {
        "DefaultDamage",
    }

    return ArrayHelper.StringArrayFromTable(List)
end

return DefaultCalculator
