### d2lootfilter

d2lootfilter is a plugin that can be used with [PlugY](http://plugy.free.fr/en/index.html) or other mod systems to filter loot on 1.10f, 1.13c, and 1.14d. The syntax for filtering loot was made to be similar to [Path of Exile](https://pathofexile.fandom.com/wiki/Item_filter).

* [Sample Filter](doc/ENG/item.filter)
* [Features](#Features)
* [Installing](#Installing)
* [Basic Syntax](#Basic-Syntax)
* [Operators](#Operators)
* [Conditions](#Conditions)
    * [Stats](#Stats)
* [Actions](#Actions)
* [Styles](#Styles)
* [Settings](#Settings)
* [In Game Commands](#In-Game-Commands)
* [Credits](#Credits)

**This has not been heavily tested, crashes may happen, and items may be lost**

### Features

* Filter loot by various attributes (still a work in progress)
* Notifications for loot
* Minimap icons for loot
* Custom name/descriptions/background colors for ground and inventory loot.

Still todo:

* Error handling. Filter just silently ignores errors right now.
* Disable sound for hidden drops
* Tier system to change filter levels in game.

Example with custom item name/border/background color/inventory background color and chat notification.

![image](https://user-images.githubusercontent.com/1458109/114068726-33ed1f00-986c-11eb-8cca-686efb629e8e.png)

See what rules in your config caused the item to be shown. Set background colors for charms/items to quickly find charms that need to be replaced etc...

![image](https://user-images.githubusercontent.com/1458109/114068812-4cf5d000-986c-11eb-9795-fd7e1d6a8683.png)

### Installing

Steps to install on PlugY:

* Download the [latest dll](https://github.com/dschu012/d2lootfilter/releases/latest/download/d2lootfilter.dll) and [item.filter](https://github.com/dschu012/d2lootfilter/blob/master/doc/ENG/item.filter)
* Copy both files to your PlugY directory.
* Change `DllToLoad=` to `DllToLoad=d2lootfilter.dll` in PlugY.ini

### Basic Syntax

The basic syntax is a collection of `Show` and `Hide` condition blocks. If an item matches **all conditions** on the block, the item will respectively be hidden or shown. `Continue` may be added to the end of a condition block if you would like the item to continue trying to match other condition blocks, otherwise processing stops after the first block is matched. A sample filter can be found [here](doc/ENG/item.filter).

```
# hides all inferior items
Hide
    Rarity Inferior

# append the rune number to all runes
Show
    Class Rune
    SetName {Name} {Red}{Rune Number}
Continue

# since continue was specified on the last condiition block
# this one will also match. the name carries over from the
# previous condiition block
Show
    Rune >= Vex
    ChatNotify True
    MinimapIcon Purple
```

### Operators

The operators are as followed. If no operator is specified `=` is implied.

| Operator | Description |
|-|-|
| `=` | Equals |
| `!=` | Not Equals |
| `<` | Less Than |
| `<=` | Less Than Equals |
| `>` | Greater Than |
| `>=` | Greater Than Equals |
| `in X-Y` | Between values X and Y |

### Conditions

* `,` essentially works as an `or` operator. i.e. `Type Swirling Crystal, Dimensional Shard` would match either item.

| Name | Valid Values |
|-|-|
| Type `<Operator> <Value>` | Name of item in `strings.txt` lookup from `weapons.txt`, `armor.txt`, or `misc.txt` . i.e. `Swirling Crystal`. For 1.13c and 1.14d these can be found [here](doc/ENG/Types.md). |
| Code `<Operator> <Value>` | 3 character item code found in `weapons.txt`, `armor.txt`, or `misc.txt`. For 1.13c and 1.14d these can be found [here](doc/ENG/Types.md). |
| Class `<Operator> <Value>` | ItemType column from `itemtypes.txt`. For 1.13c and 1.14d these can be found [here](doc/ENG/Class.md). |
| Ethereal `<Boolean>` | Boolean True or False |
| Rarity `<Rarity>` | Inferior, Normal, Superior, Magic, Set, Rare, Unique, Crafted |
| Runeword `<Boolean>` | Boolean True or False |
| Prefix `<Operator> <Value>` | Prefix ID from `magicprefix.txt` (todo... human readable name) |
| Suffix `<Operator> <Value>` | Suffix ID from `magicsuffix.txt` (todo... human readable name) |
| ItemLevel `<Operator> <Value>` | Number |
| Quality `<Operator> <Quality>` | Normal, Exceptional, Elite |
| CharacterLevel `<Operator> <Value>` | Number |
| Difficulty `<Operator> <Value>` | Normal, Nightmare, Hell |
| Rune `<Operator> <Value>` | Rune Name or Number. For 1.13c and 1.14d these can be found [here](doc/ENG/Runes.md). |
| Id `<Operator> <Value>` | Unique or Set ID from `sets.txt` or `uniques.txt` |
| Gold `<Operator> <Value>` | Gold value |
| Defense `<Operator> <Value>` | Defense |
| Price `<Operator> <Value>` | Price when vendoring item |
| Identified `<Boolean>` | Boolean True or False |
| Armor `<Boolean>` | Boolean True or False. True if item is any armor. |
| Weapon `<Boolean>` | Boolean True or False. True if item is any weapon. |
| Sockets `<Operator> <Number>` | Number of sockets |
| Width `<Operator> <Number>` | Width of item in inventory |
| Height `<Operator> <Number>` | Height of item in inventory |
| Stats `<Expression>` | Expression that evaluates to true or false to filter an item based on stats. More details can be found in [Stats](#Stats) |


### Stats

Stats are expressions that evaluate to true or false. i.e `"All Resists > 0 and "Life" > 0` would match items with both all resists and life. A list of keywords that can be used in stats expressions can be found here. [Stats](doc/ENG/Stats.md), [Skills](doc/ENG/Skills.md). These keywords must be quoted `"`.

| Functions | Description |
|-|-|
| `Stat` | Use other stats that are not in [Stats](doc/ENG/Stats.md). 2 arg function. The first arg is the stat id from `itemcoststats.txt`. The second arg is optional layer (used for skill stats). Returns the unsigned value of the stat. |
| `ChargedSkill` | Check the skill level of a charged skill. i.e. `ChargedSkill(54) > 0` will check if an item has charges of teleport |
| `ClassSkill` | Check if an item has a certain class skill. i.e. `ClassSkill(1) > 0` will check if an item has +To All Sorc skills. |
| `TabSkill` | Check if an item has a certain tab skill. i.e. `TabSkill(34) > 0` will check if an item has +To to Warcries skills. |
| `Class` | Check if an item is a certain type/class. 1 arg function of the class from `itemtypes.txt`. |
| `Min` | varargs. returns the minimum (non-zero, exclude stats that don't exist) value from a list |
| `MinIn` | varargs. returns the minimum (including zeros, i.e. stats that don't exist on the item) value from a list |
| `Max` | varargs. returns the maximum (non-zero, exclude stats that don't exist) value from a list |

e.x. `Max(Stat(39), Stat(43), Stat(41), Stat(45)) > 0` can be used to filter the existance of any resistance and `MinIn(Stat(39), Stat(43), Stat(41), Stat(45)) > 0` for all resistances

### Actions

* Name and Description can use the following color tokens. `{White}`, `{Red}`, `{Green}`, `{Blue}`, `{Gold}`, `{Gray}`, `{Black}`, `{Tan}`, `{Orange}`, `{Yellow}`, `{Purple}`, `{Dark Green}`.
* Name and Description can use the following special tokens: `{Price}` (Item Price), `{Sockets}` (Number of sockets)

| Name | Valid Values |
|-|-|
| SetStyle `<Value>` | Sets the styling for an item. A style is a group of actions that will be applied. See [Styles](#Styles). |
| SetName `<Value>` | Sets the name for an item. Special token `{Name}` is the base name for the item. When using continue it will append from the previous condition block. |
| SetDescription `<Value>` | Sets the description for an item.  Special token `{Description}` is the base name for the item. When using continue it will append from the previous condition block. |
| SetBackgroundColor `<Value>` | Sets the background color of the item when on the ground. Pallette index color or White, Red, Green, Blue, Gold, Gray, Black, Tan, Orange, Yellow, Purple, Dark Green |
| SetInventoryColor `<Value>` | Sets the background color of the item when in your inventory. Value is a pallette index color. |
| SetBorderColor `<Value>` | Sets the border color of the item when on the ground. Value is a pallette index color. |
| ChatNotify `<Boolean>` | Notify when the item drops in chat. True or False |
| MinimapIcon `<Value>` | Sets the color of the item on your minimap when on the ground. Value is a pallette index color. |

### Styles

Lets you apply a group of actions to an item. i.e.

```
Style Tier 1 Items
    SetName {Purple}T1 {Name}
    ChatNotify True
    MinimapIcon Purple
    SetInventoryColor Purple

Show
    Type Diadem
    Rarity Unique
    SetStyle Tier 1 Items

Show
    Type Unearthed Wand
    Rarity Unique
    SetStyle Tier 1 Items
```

will apply all of the `Tier 1 Items` styles to the items.

### Settings

The first time you join a game with the plugin loaded it should create a `d2lootfilter.ini` settings file in your Diablo II directory. These are the following settings that can be changed.

| Setting | Description |
|-|-|
| Path | Path to your filter file. Default: `./item.filter` |
| FilterLevel | Used to dynamically change how strict your filter while playing. (Currently unused, still a planned feature) Default: `6` |
| PingLevel | Used to dynamically change how strict drop notification are while playing. Default: `6` |


### In Game Commands

The filter has a few in-game commands for changing settings.

| Command | Description |
|-|-|
| `/reload` | Reloads your filter. |
| `/debug` | Toggles debugging. |
| `/test <number>` | Tests a specific rule (by line number) against your currently hovered mouse item. |
| `/filterlevel <number>` | Change the filter level. |
| `/pinglevel <number>` | Change the ping/notification level. |


### Credits

Special thanks to everyone that has shared their work at [Phrozen-Keep](https://d2mods.info/forum/viewforum.php?f=8) [(Discord)](https://discord.gg/NvfftHY).

To name a few Necrolis, Lectem, Kingpin, whist, Revan, etc...

Thanks to coffin_spirit on Discord for the 1.10f implementation.
