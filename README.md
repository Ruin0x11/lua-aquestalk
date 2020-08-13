# lua-aquestalk

A Lua 5.1/LuaJIT binding to [AquesTalk 10](https://www.a-quest.com/products/aquestalk10.html), most commonly known as the source for the "Yukkuri Voice".

## Example

See `main.lua` for a complete example that runs under LÖVE 11.3.

```lua
local aquestalk = require "lua_aquestalk"

-- Set the license key.
-- aquestalk.set_usr_key("XXXXX-XXXXX-XXXXX")
-- aquestalk.set_dev_key("YYYYY-YYYYY-YYYYY")

local bas = aquestalk.VoiceBase.F1E -- F1E, F2E, M1E
local spd = 100 -- 50-300
local vol = 100 -- 0-300
local pit = 100 -- 20-200
local acc = 100 -- 0-200
local lmd = 100 -- 0-200
local fsc = 100 -- 50-200

-- Takes UTF-8 as input (no kanji).
local wav, err = aquestalk.synth(text, bas, spd, vol, pit, acc, lmd, fsc)

local filedata = love.filesystem.newFileData(wav, "aquestalk.wav")
local source = love.audio.newSource(filedata, "static")

source:play()

```

## Building

Download AquesTalk 10 from the [offical website](https://www.a-quest.com/products/aquestalk10.html) and extract the files to `lib` under `lib/aqtk10_win` or `lib/aqtk10_lnx` depending on the platform.

Then, compile the library:

```
mkdir build
cd build
cmake ..
make
```

Afterwards, copy `lua_aquestalk.dll`/`lua_aquestalk.so` and `AquesTalk.dll`/`libAquesTalk10.so.1` somewhere.

## Notes

AquesTalk is proprietary software. You can use it without a license, but certain phonemes in the synthesized output will be replaced.
