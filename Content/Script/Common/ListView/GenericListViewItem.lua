require "UnLua"
require "Common.TableUtils"

---@field ParentWrapper ListViewWrapper Parent wrapper of this item.
---@class GenericListViewItem Generic list item for storing data.
local GenericListViewItem = Class()

---Initialize item data with parent list wrapper.
---@param InWrapper ListViewWrapper Parent list wrapper of this item.
function GenericListViewItem:InitializeWithList(InWrapper)
    self.ParentWrapper = InWrapper
end

---Set the carrying data set.
---@param InData table Table of stores data.
function GenericListViewItem:SetData(InData)
    if type(InData) ~= "table" then
        self.Data = {InData}

        ---@type boolean Indicates the data in this item is automatically packed.
        self.bPacked = true
    else
        self.Data = InData
    end
end

---Get the carrying data set.
---@return table|number|boolean|string Value of stores data.
function GenericListViewItem:GetData()
    -- Unpack the value automatically
    if self.bPacked then
        return self.Data[1]
    else
        return self.Data
    end
end

return GenericListViewItem