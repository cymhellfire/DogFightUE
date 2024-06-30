---@class BitField Class that handle bit field.
local BitField = {}

---Add a bit to given field.
---@param Field number Target field.
---@param Bit number Bit to add.
function BitField.AddBit(Field, Bit)
    if type(Field) ~= "number" or type(Bit) ~= "number" then
        return
    end

    return Field | Bit
end

---Remove a bit from given field.
---@param Field number Target field.
---@param Bit number Bit to remove.
function BitField.RemoveBit(Field, Bit)
    if type(Field) ~= "number" or type(Bit) ~= "number" then
        return
    end

    return Field & (~Bit)
end

---Check whether field contains given bits.
---@param Field number Target field.
---@param Bit number Bit to check.
function BitField.HasBit(Field, Bit)
    if type(Field) ~= "number" or type(Bit) ~= "number" then
        return
    end

    return Bit == (Field & (~Bit))
end

return BitField