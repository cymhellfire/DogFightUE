require "Common.TableUtils"

local ViewModelBase = {}

local function NewIndex(t, k, v)
    t:__TriggerDelegate(k, v)
    rawset(t.BindingTable, k, v)
end

local function Index(t, k)
    return rawget(t.BindingTable, k)
end

---Create a new ViewModel based on given prototype.
---@param InViewModel table ViewModel prototype
---@return table New ViewModel instance
local function InstantiateViewModel(InViewModel)
    local NewVM = table.deepCopy(ViewModelBase)
    -- Append the binding table to new ViewModel
    if InViewModel and InViewModel["BindingTable"] then
        NewVM["BindingTable"] = InViewModel["BindingTable"]
    end
    return NewVM
end

function ViewModelBase:SetupBinding(InView, BindTable)
    local mt = {
        __newindex = NewIndex,
        __index = Index,
    }
    setmetatable(self, mt)

    rawset(self, "BindView", InView)
    rawset(self, "DelegateMap", {})

    if not self.BindingTable then
        return
    end

    for _, v in ipairs(BindTable) do
        self:AddBinding(v)
    end
end

function ViewModelBase:AddBinding(InTable)
    local BindKey = InTable.BindKey
    if not BindKey then
        return
    end

    -- Skip the non-exist binding
    local BindingTable = rawget(self, "BindingTable")
    if not table.containsKey(BindingTable, BindKey) then
        return
    end

    local DelegateMap = rawget(self, "DelegateMap")
    local TargetBind = DelegateMap[BindKey]
    -- Create new delegate map
    if not TargetBind then
        DelegateMap[BindKey] = {}
        TargetBind = DelegateMap[BindKey]
    end

    -- Add new listener
    TargetBind[#TargetBind + 1] = {
        ["UIKey"] = InTable.UIKey,
        ["DataBinding"] = InTable.DataBinding,
    }
end

function ViewModelBase:__TriggerDelegate(key, value)
    local DelegateMap = rawget(self, "DelegateMap")
    local ViewWidget = rawget(self, "BindView")
    local TargetBind = DelegateMap[key]
    if TargetBind and #TargetBind > 0 then
        for _, v in ipairs(TargetBind) do
            local Widget = v["UIKey"]
            local DataPipe = v["DataBinding"]
            if DataPipe and Widget then
                DataPipe.SetValue(ViewWidget[Widget], value)
            end
        end
    end
end

_G.InstantiateViewModel = InstantiateViewModel

return ViewModelBase
