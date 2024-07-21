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

local AnimCfg = {
    {
        DisplayName = "Attack 1",
        Enum = UE.EActionAnimPredefinedType.Attack1,
    },
    {
        DisplayName = "Attack 2",
        Enum = UE.EActionAnimPredefinedType.Attack2,
    }
}

function CharacterPreviewView:PostInitialized()
    local NewVM = InstantiateViewModel(CharacterPreviewVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding() }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.CharacterSelect_ComboBox.OnSelectionChanged:Add(self, self.OnCharacterSelectChanged)
    self.AnimSelect_ComboBox.OnSelectionChanged:Add(self, self.OnAnimSelectChanged)
    self.PlayAnim_Button.OnClicked:Add(self, self.OnPlayAnimClicked)

    self:InitActionPreviewList()

    ---@type TimerService
    local TimerService = GetGameService(self, GameServiceNameDef.TimerService)
    if TimerService then
        TimerService:RegisterTimer(self, self.OnInitTimerExpired, 0.5)
    end
end

function CharacterPreviewView:UnInitialize()
    self.CharacterSelect_ComboBox.OnSelectionChanged:Remove(self, self.OnCharacterSelectChanged)
    self.AnimSelect_ComboBox.OnSelectionChanged:Remove(self, self.OnAnimSelectChanged)
    self.PlayAnim_Button.OnClicked:Remove(self, self.OnPlayAnimClicked)
end

function CharacterPreviewView:OnInitTimerExpired()
    SyncPreviewCharacter(self)
end

function CharacterPreviewView:OnCharacterSelectChanged(Item, SelectionType)
    print("CharacterPreviewView:OnCharacterSelectChanged ", Item)
    
    SyncPreviewCharacter(self)
end

function CharacterPreviewView:OnAnimSelectChanged(Item, SelectionType)
    print("CharacterPreviewView:OnAnimSelectChanged", Item)

    self.CurSelectAnimIndex = self.AnimSelect_ComboBox:GetSelectedIndex() + 1
end

function CharacterPreviewView:PreviewAvatar(InConfigId)
    ---@type CharacterPreviewController
    local PlayerController = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerController(self)
    if PlayerController then
        PlayerController:ChangePreviewCharacter(InConfigId)
    end
end

function CharacterPreviewView:InitActionPreviewList()
    for _, v in ipairs(AnimCfg) do
        self.AnimSelect_ComboBox:AddOption(v.DisplayName)
    end

    self.AnimSelect_ComboBox:SetSelectedIndex(0)
end

function CharacterPreviewView:OnPlayAnimClicked()
    if not self.CurSelectAnimIndex then
        return
    end

    ---@type CharacterPreviewController
    local PlayerController = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerController(self)
    ---@type ATopDownStylePlayerCharacter
    local PlayerCharacter = PlayerController and PlayerController:GetCharacterPawn()
    local SelectAnimCfg = AnimCfg[self.CurSelectAnimIndex]
    if PlayerCharacter and SelectAnimCfg then
        UE.UAvatarFunctionLibrary.PlayPredefineAnimWithAvatar(PlayerCharacter, SelectAnimCfg.Enum)
    end
end

return CharacterPreviewView