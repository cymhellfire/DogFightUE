local CardSymbolDef = require "DogFight.Services.CardService.CardSymbolDef"
local CardEssensialDef = require "DogFight.Services.CardService.CardEssentialDef"

local DefaultCardPoolCfg = {}

local CardSequenceOne = {
    [CardEssensialDef.Attack] = {1, 2, 3, 4, 11},
    [CardEssensialDef.Defence] = {5, 6, 7, 8, 12},
    [CardEssensialDef.Heal] = {9, 10},
    [CardEssensialDef.Universal] = {13},
}

local CardSequenceTwo = {
    [CardEssensialDef.Attack] = {1, 2, 3, 4, 12},
    [CardEssensialDef.Defence] = {5, 6, 7, 8, 11},
    [CardEssensialDef.Heal] = {9, 10},
    [CardEssensialDef.Universal] = {13},
}

DefaultCardPoolCfg.Cards = {
    [CardSymbolDef.Hearts] = CardSequenceOne,
    [CardSymbolDef.Diamonds] = CardSequenceTwo,
    [CardSymbolDef.Spades] = CardSequenceOne,
    [CardSymbolDef.Clubs] = CardSequenceTwo,
}

DefaultCardPoolCfg.NumberToText = {
    [1] = "A",
    [11] = "J",
    [12] = "Q",
    [13] = "K",
}

DefaultCardPoolCfg.SymbolSettings = {
    [CardSymbolDef.Hearts] = {
        IconPath = "/Game/DogFightGame/Texture/UI/Tex_Poker_Hearts.Tex_Poker_Hearts",
        Color = { R = 1, G = 0, B = 0, A = 1, },
    },
    [CardSymbolDef.Diamonds] = {
        IconPath = "/Game/DogFightGame/Texture/UI/Tex_Poker_Diamonds.Tex_Poker_Diamonds",
        Color = { R = 1, G = 0, B = 0, A = 1, },
    },
    [CardSymbolDef.Spades] = {
        IconPath = "/Game/DogFightGame/Texture/UI/Tex_Poker_Spades.Tex_Poker_Spades",
        Color = { R = 0, G = 0, B = 0, A = 1, },
    },
    [CardSymbolDef.Clubs] = {
        IconPath = "/Game/DogFightGame/Texture/UI/Tex_Poker_Clubs.Tex_Poker_Clubs",
        Color = { R = 0, G = 0, B = 0, A = 1, },
    }
}

return DefaultCardPoolCfg