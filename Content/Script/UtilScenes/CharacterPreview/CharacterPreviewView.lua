---@class CharacterPreviewView : ModelBase 
local CharacterPreviewView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local CharacterPreviewVM = require("UtilScenes.CharacterPreview.CharacterPreviewVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

local function SyncPreviewCharacter(self)
    local Index = self.CharacterSelect_ComboBox:GetSelectedIndex()
    self:PreviewAvatar(Index + 1)
end

function CharacterPreviewView:PostInitialized()
    local NewVM = InstantiateViewModel(CharacterPreviewVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding() }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.CharacterSelect_ComboBox.OnSelectionChanged:Add(self, self.OnCharacterSelectChanged)

    SyncPreviewCharacter(self)
end

function CharacterPreviewView:UnInitialize()
    self.CharacterSelect_ComboBox.OnSelectionChanged:Remove(self, self.OnCharacterSelectChanged)
end

function CharacterPreviewView:OnCharacterSelectChanged(Item, SelectionType)
    print("CharacterPreviewView:OnCharacterSelectChanged ", Item)
    
    SyncPreviewCharacter(self)
end

function CharacterPreviewView:PreviewAvatar(InConfigId)
    ---@type CharacterPreviewController
    local PlayerController = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerController(self)
    if PlayerController then
        PlayerController:ChangePreviewCharacter(InConfigId)
    end
end

return CharacterPreviewView