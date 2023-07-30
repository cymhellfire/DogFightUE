---@class BuffBase : UNewBuffBase
---@field _Logic BuffLogicBase Buff logic script class.
local BuffBase = UnrealClass()

function BuffBase:LoadBuffScript(InPath)
    local LogicTemp = require(InPath)
    if LogicTemp then
        self._Logic = LogicTemp:New()
        self._Logic:Init(self)
    end
end

function BuffBase:OnBuffInitScript()
    if self._Logic then
        self._Logic:OnInit()
    end
end

---Initialize buff with given arguments
---@param InArgs table
function BuffBase:SetupArgument(InArgs)
    if self._Logic then
        self._Logic:SetupArgument(InArgs)
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffBase:ApplyScript(InCharacter)
    if self._Logic then
        self._Logic:OnApply(InCharacter)
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffBase:RemoveScript(InCharacter)
    if self._Logic then
        self._Logic:OnRemove(InCharacter)
    end
end

---@param NewCharacter ATopDownStylePlayerCharacter
function BuffBase:TransferToScript(NewCharacter)
    if self._Logic then
        self._Logic:OnTransferTo(NewCharacter)
    end
end

function BuffBase:OnBuffFinishScript()
    if self._Logic then
        self._Logic:OnFinish()
    end
end

---@param InType EBuffCheckType
function BuffBase:OnDoBuffCheckScript(InType)
    if self._Logic then
        self._Logic:DoCheck(InType)
    end
end

return BuffBase