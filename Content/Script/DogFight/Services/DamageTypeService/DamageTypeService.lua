require "UnLua"

---@class DamageTypeService : GameServiceBase Service that hold all damage type data.
local DamageTypeService = Class("DogFight.Services.GameServiceBase")
local DamageTypeNameDef = require("DogFight.Services.DamageTypeService.DamageTypeNameDef")

function DamageTypeService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    table.dump(self.Config)
end

function DamageTypeService:GetConfigPath()
    return "DogFight.Services.DamageTypeService.DamageTypeConfig"
end

return DamageTypeService