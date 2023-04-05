require "Common.ListView.GenericListViewItem"
require "LuaIntegration.Common.UnrealUtils"

---@field ParentWidget UListView The ListView this wrapper works for
---@field ParentView UUserWidget The container widget of this list view.
---@field DataObjectList table Stores references to all list item object.
---@class ListViewWrapper Wrapper class of ListView operations.
local ListViewWrapper = {}

local ClassPath = "/Game/DogFightGame/UI/Common/BP_Data_CommonListItemObject.BP_Data_CommonListItemObject_C"

---Create a new wrapper for specified ListView.
---@param InWidget UUserWidget Container widget of the list view.
---@param InListView UListView List view instance that wrapper work with.
---@return ListViewWrapper New list view wrapper.
function ListViewWrapper.New(InWidget, InListView)
    local NewWrapper = table.deepCopy(ListViewWrapper)

    -- Initialize new wrapper with ListView
    if NewWrapper then
        NewWrapper:Initialize(InListView, InWidget)
    end

    return NewWrapper
end

---Initialize wrapper with given ListView.
---@param InListView UListView ListView instance to initialize with.
---@param InWidget UUserWidget Container widget of the list view.
function ListViewWrapper:Initialize(InListView, InWidget)
    self.ParentWidget = InListView
    self.ParentView = InWidget

    self.DataObjectList = {}
end

---Create a new UObject that holds the given data.
---@param self ListViewWrapper
---@param Data any Data to be hold
---@return GenericListViewItem New object instance
local function ConstructWrapperObjectWithData(self, Data)
    ---@type GenericListViewItem
    local NewItemObject = NewObject(self.ItemClass, self.ParentWidget)
    NewItemObject:InitializeWithList(self)
    NewItemObject:SetData(Data)

    -- Record to data item list and add a reference to it to prevent GC
    self:AddObjectRef(NewItemObject)

    return NewItemObject
end

---Load item data contains in a list.
---@param InDataList table Table of all item data
function ListViewWrapper:LoadDataByList(InDataList)
    if self.ItemClass == nil then
        ---@type UClass Item class to spawn.
        self.ItemClass = LoadClass(ClassPath)
    end

    if self.ItemClass then
        -- Clear exists items first
        self:ClearObjectRef()
        ---@type TArray
        local DataItemList = UE.TArray(UE.UObject)
        if InDataList then
            -- Create UObject for carrying the data
            for i = 1, #InDataList do
                -- Record new item to array
                DataItemList:Add(ConstructWrapperObjectWithData(self, InDataList[i]))
            end
        end

        -- Set object to unreal side
        if self.ParentWidget then
            if DataItemList:Length() > 0 then
                self.ParentWidget:BP_SetListItems(DataItemList)
            else
                self.ParentWidget:ClearListItems()
            end
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

---Add a new data object to this list.
---@param InData any The data add to this list.
function ListViewWrapper:AddDataObject(InData)
    if self.ItemClass == nil then
        ---@type UClass Item class to spawn.
        self.ItemClass = LoadClass(ClassPath)
    end

    if self.ItemClass then
        local NewItem = ConstructWrapperObjectWithData(self, InData)

        -- Add to native list view
        if self.ParentWidget then
            self.ParentWidget:AddItem(NewItem)
        end
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