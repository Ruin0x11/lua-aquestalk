local src_dir, build_dir = ...
if ( src_dir ) then
    package.path  = src_dir .. "?.lua;" .. package.path
    package.cpath = build_dir .. "?.so;" .. build_dir .. "?.dll;" .. package.cpath
end

local aquestalk = require 'lua_aquestalk'

print(aquestalk.VoiceBase.F1E)
print(aquestalk.VoiceBase.F2E)
print(aquestalk.VoiceBase.M1E)
