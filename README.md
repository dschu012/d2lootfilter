### d2lootfilter

d2lootfilter is a plugin that can be used with [PlugY](http://plugy.free.fr/en/index.html) or other mod systems to filter loot on 1.13c and 1.14d. The syntax for filtering loot was made to be similar to Path of Exile.

* [Sample Filter](doc/ENG/item.filter)
* [Features](#Features)
* [Installing](#Installing)
* [Basic Syntax](#Basic-Syntax)
* [Operators](#Operators)
* [Conditions](#Conditions)
    * [Stats](#Stats)
* [Actions](#Actions)
* [Credits](#Credits)

**This has not been heavily tested, crashes may happen, and items may be lost**

### Features

* Filter loot by various attributes (still a work in progress)
* Notifications for loot
* Minimap icons for loot
* Custom name/descriptions/background colors for ground and inventory loot.

Still todo:

* Error handling. Filter just silently ignores errors right now.
* UI for enabling/disabling debugging features (temporarily show hidden items and show what conditions caused them being hidden). Currently Ctrl + Shift + Z disabled the filter and shows debug information.
* Tier system to change filter levels in game.

Example with custom item name/border/background color/inventory background color and chat notification.

![image](https://user-images.githubusercontent.com/1458109/114068726-33ed1f00-986c-11eb-8cca-686efb629e8e.png)

See what rules in your config caused the item to be shown. Set background colors for charms/items to quickly find charms that need to be replaced etc...

![image](https://user-images.githubusercontent.com/1458109/114068812-4cf5d000-986c-11eb-9795-fd7e1d6a8683.png)

### Installing

TODO

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
| SetName `<Value>` | Special token `{NAME}` is the base name for the item. When using continue it will append from the previous condition block. |
| SetDescription `<Value>` | Special token `{NAME}` is the base name for the item. When using continue it will append from the previous condition block. |
| SetBackgroundColor `<Value>` | Pallette index color or White, Red, Green, Blue, Gold, Gray, Black, Tan, Orange, Yellow, Purple, Dark Green |
| SetInventoryColor `<Value>` | Same as SetBackgroundColor |
| SetBorderColor `<Value>` | Same as SetBackgroundColor |
| ChatNotify `<Boolean>` | True or False |
| MinimapIcon `<Value>` | Same as SetBackgroundColor |

### Credits

Special thanks to everyone that has shared their work at [Phrozen-Keep](https://d2mods.info/forum/viewforum.php?f=8) [(Discord)](https://discord.gg/NvfftHY).

To name a few Necrolis, Lectem, Kingpin, whist, Revan, etc...
