---@field Data table Raw config data table.
---@field _DataNum number Total number of config.
---@class ServiceConfigBase Base class of all service config.
local ServiceConfigBase = UnrealClass()

function ServiceConfigBase:ctor()
    self._DataNum = 0
    -- Get data and load
    if type(self.GetData) == "function" then
        local Data = self:GetData()
        self:LoadData(Data)
    end
end

---Initialize the loaded data
---@param self ServiceConfigBase ServiceConfig instance.
local function InitializeLoadedData(self)
    -- Look up in config data
    for k, v in pairs(self.Data) do
        -- Sync id
        if type(v.Id) ~= "number" then
            v.Id = k
        end
        -- Set default name
        if type(v.Name) ~= "string" then
            v.Name = "Unnamed"
        end

        -- Add to config count
        self._DataNum = self._DataNum + 1
    end
end

---Load a list of data to this config.
---@param InData table List of config data.
function ServiceConfigBase:LoadData(InData)
    if not InData then
        return
    end

    self.Data = InData
    -- Init data
    InitializeLoadedData(self)
end

---Get the total count of config data.
---@return number Total amount of config item.
function ServiceConfigBase:GetConfigNum()
    if not self.Data then
        return 0
    end

    return self._DataNum
end

---Get config with specified id.
---@param InId number Id of config data.
---@return table Config data.
function ServiceConfigBase:GetConfig(InId)
    if not self.Data then
        return
    end

    if InId then
        return self.Data[InId]
    end
end

return ServiceConfigBase