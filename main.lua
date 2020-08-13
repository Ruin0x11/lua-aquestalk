local aquestalk = require "lua_aquestalk"

-- Set the license key.
aquestalk.set_usr_key("XXXXX-XXXXX-XXXXX")
aquestalk.set_dev_key("YYYYY-YYYYY-YYYYY")

local bas = aquestalk.VoiceBase.F1E -- F1E, F2E, M1E
local spd = 100 -- 50-300
local vol = 100 -- 0-300
local pit = 100 -- 20-200
local acc = 100 -- 0-200
local lmd = 100 -- 0-200
local fsc = 100 -- 50-200

local function play_voice(text)
    -- Takes UTF-8 as input (no kanji).
    local wav, err = aquestalk.synth(text, bas, spd, vol, pit, acc, lmd, fsc)

    local filedata = love.filesystem.newFileData(wav, "aquestalk.wav")
    local source = love.audio.newSource(filedata, "static")

    source:play()
end

function love.keypressed(key)
    if key == "p" then
        play_voice("ゆっくりしてね")
    end
end

function love.update()
end

function love.draw()
    love.graphics.print("Press 'p' to yukkuri.", 10, 10)
end
