require "UnLua"

local DamageCalculatorService = Class("DogFight.Services.GameServiceBase")

function DamageCalculatorService:LoadDamageCalculator(InPath, Instigator)
    local DefaultClass = self:GetDamageCalculatorClass()
    if DefaultClass == nil then
        return
    end

    local NewInstance = NewObject(DefaultClass, Instigator, nil, self:GetCalculatorScriptPath(InPath))
    return NewInstance
end

function DamageCalculatorService:LoadDamageInstance(InPath, Instigator)
    local DefaultClass = self:GetDamageInstanceClass()
    if DefaultClass == nil then
        return
    end

    local NewInstance = NewObject(DefaultClass, Instigator, nil, self:GetInstanceScriptPath(InPath))
    return NewInstance
end

function DamageCalculatorService:GetCalculatorScriptPath(InPath)
    return "DogFight.DamageCalculator." .. InPath
end

function DamageCalculatorService:GetInstanceScriptPath(InPath)
    return "DogFight.DamageInstance." .. InPath
end

return DamageCalculatorService
