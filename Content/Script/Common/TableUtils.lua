---Find specified key in given table.
---@param table table Table to search with.
---@param key any Key to search.
---@return boolean Whether the key is exist.
function table.containsKey(table, key)
    local bFind = false

    if type(table) == "table" then
        for k, _ in pairs(table) do
            if k == key then
                bFind = true
                break
            end
        end
    end

    return bFind
end

---Find specified value in given table.
---@param table table Table to search with.
---@param value any Value to search.
---@return boolean Whether the value is exist.
function table.containsValue(table, value)
    local bFind = false

    if type(table) == "table" then
        for _, v in pairs(table) do
            if v == value then
                bFind = true
                break
            end
        end
    end

    return bFind
end

---Remove single given value from table.
function table.removeValue(tab, value)
    for i, v in ipairs(tab) do
        if v == value then
            table.remove(tab, i)
            break
        end
    end
end

local function deepCopy(org, copies)
    copies = copies or {}
    local orgType = type(org)
    local copy
    if orgType == "table" then
        if copies[org] then
            copy = copies[org]
        else
            copy = {}
            copies[org] = copy
            for orgKey, orgValue in next, org, nil do
                copy[deepCopy(orgKey, copies)] = deepCopy(orgValue, copies)
            end
            setmetatable(copy, deepCopy(getmetatable(org), copies))
        end
    else
        -- Handle simple type
        copy = org
    end
    return copy
end

---Deep copy given table and return new one
---@param orgTab table Table to copy
---@return table New copied table
function table.deepCopy(orgTab)
    return deepCopy(orgTab)
end

---Print all elements in given table.
---@param inTable table Table to dump
function table.dump(inTable)
    if type(inTable) ~= "table" then
        return
    end

    for k, v in pairs(inTable) do
        print(k .. ": " .. v)
    end
end