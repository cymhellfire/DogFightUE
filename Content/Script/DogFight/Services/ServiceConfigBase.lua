require "UnLua"

---@field Data table Raw config data table.
---@field IdToNameCache table Table that map config id to config name.
---@class ServiceConfigBase Base class of all service config.
local ServiceConfigBase = Class()

function ServiceConfigBase:ctor()
    self.IdToNameCache = {}

    -- Get data and load
    if type(self.GetData) == "function" then
        local Data = self:GetData()
        self:LoadData(Data)
    end
end

---Initialize the map between config id and name.
---@param self ServiceConfigBase ServiceConfig instance.
local function InitializeIdToNameCache(self)
    -- Look up in config data
    for k, v in pairs(self.Data) do
        if not self.IdToNameCache[v.Id] then
            self.IdToNameCache[v.Id] = k
            -- Set default name
            if type(v.Name) ~= "string" then
                v.Name = k
            end
        else
            print("[ServiceConfig] Duplicate config Id detected: " .. v.Id)
        end
    end
end

---Load a list of data to this config.
---@param InData table List of config data.
function ServiceConfigBase:LoadData(InData)
    if not InData then
        return
    end

    self.Data = InData
    -- Init map
    InitializeIdToNameCache(self)
end

---Get config with specified name.
---@param InName string Name of config data.
---@return table Config data.
function ServiceConfigBase:GetConfigByName(InName)
    if not self.Data then
        return
    end

    return self.Data[InName]
end

---Get config with specified id.
---@param InId number Id of config data.
---@return table Config data.
function ServiceConfigBase:GetConfigById(InId)
    local Name = self.IdToNameCache[InId]
    if Name then
        return self.Data[Name]
    end
end

---Get the id of config with given name.
---@param InName string Config name.
---@return number Config Id.
function ServiceConfigBase:NameToId(InName)
    local Config = self.Data[InName]
    if Config then
        return Config.Id
    end
end

---Get the name of config with given id.
---@param InId number Config Id.
---@return string Config name.
function ServiceConfigBase:IdToName(InId)
    return self.IdToNameCache[InId]
end

return ServiceConfigBase