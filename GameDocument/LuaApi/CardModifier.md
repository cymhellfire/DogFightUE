# 卡牌修改器

卡牌修改器API包括创建修改器及修改器本身脚本接口。

<!-- TOC -->
* [卡牌修改器](#卡牌修改器)
  * [创建新的卡牌修改器](#创建新的卡牌修改器)
    * [获取修改器服务](#获取修改器服务)
    * [CreateCardModifier函数](#createcardmodifier函数)
  * [卡牌修改器脚本](#卡牌修改器脚本)
    * [基本结构](#基本结构)
    * [自定义修改器效果](#自定义修改器效果)
<!-- TOC -->

## 创建新的卡牌修改器

卡牌修改器需要创建出来才可以应用到卡牌上。

### 获取修改器服务

卡牌修改器由*CardModifierGameService*进行创建，可在全局作用域中*GameServices*表中访问。<br>
代码样例：
```
--获取修改器服务
local ModifierService = _G.GameServices.CardModifierGameService
```

### CreateCardModifier函数

调用`CreateCardModifier(Path, Instigator)`函数可以创建新的卡牌修改器。

|    参数名称    | 功能           | 备注         |
|------------|--------------|------------|
|    Path    | 指定修改器使用的脚本名称 |            |
| Instigator | 修改器创建的发起者    | 基本使用self即可 |

代码样例：
```
--获取修改器服务
local ModifierService = _G.GameServices.CardModifierGameService
if ModifierService then
    -- 创建新的修改器
    local Modifier1 = ModifierService:CreateCardModifier("CardModifierTest", self)
end
```

## 卡牌修改器脚本

脚本定义了卡牌修改器生效后的修改效果。

### 基本结构

所有的卡牌修改器脚本都遵循一个基本结构，如下所示：

```
require "Unlua"

--定义修改器类型
local XXX = Class("DogFight.CardModifiers.CardModifierBase")

--其他代码

--返回定义的类型
return XXX
```

例子中XXX为修改器类型名称，推荐使用包括*CardModifier*关键字的名称以提高辨识度，如*CardModifierDoubleDamage*。

### 自定义修改器效果

卡牌修改器的效果由创建时传入的参数集确定，该参数集可在`GetArgumentTable()`函数中修改。下面的例子展示了一个将整数属性值乘以2的修改器的参数集。

```
function XXX:GetArgumentTable()
    return {
        ["DataType"] = "Integer",
        ["OperatorType"] = "Multiply",
        ["Value"] = 2,
    }
end
```

这里的*Argument*表包含了所有创建修改器所需的参数，这些参数决定了修改器的行为。

| 参数名          | 功能         | 备注                                                             |
|--------------|------------|----------------------------------------------------------------|
| DataType     | 指定修改器的数据类型 | 从Boolean（布尔），Integer（整数）与Float（小数）中选择，不填将自动指定None类型导致无法生效。     |
| Value        | 指定修改器的修改参数 | 需要确保输入的值与DataType匹配，不填或不匹配时将使用DataType对应的默认值（布尔为false，数字型为0）。  |
| OperatorType | 指定修改器的运算类型 | 从Set（赋值），Add（加），Sub（减），Multiply（乘）和Divide（除）中选择，不填将自动指定Set类型。  |
| ApplyRule    | 修改器应用规则    | 该规则定义了修改器可以应用在卡牌的哪些属性上，不填则可以应用到任何数据类型匹配的属性上。                   |

> 修改器应用规则可以参考 [修改器应用规则](ModifierApplyRule.md) 文档。

