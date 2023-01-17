require "Common.ListView.GenericListViewItem"
require "LuaIntegration.Common.UnrealUtils"

---@class ListViewWrapper Wrapper class of ListView operations.
local ListViewWrapper = {}

local ClassPath = "/Game/DogFightGame/UI/Common/BP_Data_CommonListItemObject.BP_Data_CommonListItemObject_C"

function ListViewWrapper.New(InListView)
    local NewWrapper = table.deepCopy(ListViewWrapper)

    -- Initialize new wrapper with ListView
    if NewWrapper then
        NewWrapper:Initialize(InListView)
    end

    return NewWrapper
end

---Initialize wrapper with given ListView.
---@param InListView UListView ListView instance to initialize with.
function ListViewWrapper:Initialize(InListView)
    ---@type UListView The ListView this wrapper works for
    self.ParentWidget = InListView

    ---@type table Stores references to all list item object.
    self.DataObjectList = {}
end

---Load item data contains in a list.
---@param InDataList table Table of all item data
function ListViewWrapper:LoadDataByList(InDataList)
    if self.ItemClass == nil then
        ---@type UClass Item class to spawn.
        self.ItemClass = LoadClass(ClassPath, self.ParentWidget)
    end

    if self.ItemClass then
        -- Clear exists items first
        self:ClearObjectRef()
        local DataItemList = UE.TArray(UE.UObject)
        -- Create UObject for carrying the data
        for i = 1, #InDataList do
            ---@type GenericListViewItem
            local NewItemObject = NewObject(self.ItemClass, self.ParentWidget)
            NewItemObject:SetData(InDataList[i])
            -- Record new item to array
            DataItemList:Add(NewItemObject)

            -- Record to data item list and add a reference to it to prevent GC
            self:AddObjectRef(NewItemObject)
        end

        -- Set object to unreal side
        if self.ParentWidget then
            self.ParentWidget:BP_SetListItems(DataItemList)
        end
    end
end

---Clear all items in this list.
function ListViewWrapper:Clear()
    if self.ParentWidget then
        -- Clear the list by loading empty list
        self:LoadDataByList(nil)
    end
end

---Add a reference to given UObject to prevent it from being GCed.
---@param InObject UObject UObject instance to keep reference.
function ListViewWrapper:AddObjectRef(InObject)
    if InObject then
        UnLua.Ref(InObject)
        -- Also record in table for later removal
        self.DataObjectList[#self.DataObjectList + 1] = InObject
    end
end

---Clear the reference to spawned UObject.
function ListViewWrapper:ClearObjectRef()
    if #self.DataObjectList > 0 then
        for _, v in ipairs(self.DataObjectList) do
            UnLua.Unref(v)
        end

        self.DataObjectList = {}
    end
end

return ListViewWrapper