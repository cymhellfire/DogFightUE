local WarheadNameDef = require("DogFight.Services.WarheadService.WarheadNameDef")

---@class WarheadConfig Warhead config table.
local WarheadConfig = {}

WarheadConfig = {
    [WarheadNameDef.Default] = {
        Id = 0,
        Name = WarheadNameDef.Default,
        EffectId = 0,
    },
    [WarheadNameDef.Nuke] = {
        Id = 1,
        Name = WarheadNameDef.Nuke,
        EffectId = 1,
    },
}

return WarheadConfig