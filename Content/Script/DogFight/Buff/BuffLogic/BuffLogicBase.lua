---@class BuffLogicBase
---@field _Owner BuffBase Owner buff instance.
---@field _ModList table List of active buff mods.
local BuffLogicBase = UnrealClass()

---@param InBuff BuffBase Owner buff instance.
function BuffLogicBase:Init(InBuff)
    self._Owner = InBuff
    self._ModList = {}
end

function BuffLogicBase:OnInit()
    print("[BuffLogic] " .. self:tostring() .. " inited.")
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffLogicBase:OnApply(InCharacter)
    print("[BuffLogic] " .. self:tostring() .. " applied.")
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffLogicBase:OnRemove(InCharacter)
    print("[BuffLogic] " .. self:tostring() .. " removed.")
end

---@param NewCharacter ATopDownStylePlayerCharacter
function BuffLogicBase:OnTransferTo(NewCharacter)
    print("[BuffLogic] " .. self:tostring() .. " transfer.")
end

function BuffLogicBase:OnFinish()
    print("[BuffLogic] " .. self:tostring() .. " finished.")

    -- Clear all active mods
    if #self._ModList > 0 then
        for _, v in ipairs(self._ModList) do
            v:OnUninit()
        end

        self._ModList = {}
    end
end

---@param InType EBuffCheckType
function BuffLogicBase:DoCheck(InType)
    -- Directly mark this check passed as default
    self._Owner:FinishDoCheck()
end

local BuffModPath = "DogFight.Buff.BuffMod."

---@param InPath string Path of mod script.
function BuffLogicBase:AddMod(InPath)
    local ModTemp = require(BuffModPath .. InPath)
    if ModTemp then
        ---@type BuffModBase
        local NewMod = ModTemp:New()
        if NewMod then
            NewMod:OnInit(self)

            -- Record to mod list
            self._ModList[#self._ModList + 1] = NewMod 
        end
    end
end

function BuffLogicBase:tostring()
    return "BuffLogicBase"
end

return BuffLogicBase