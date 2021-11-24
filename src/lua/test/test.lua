function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

require('Cyperus')
local cyperus
cyperus = Cyperus.new('5000', '127.0.0.1', '5001')

local root_bus
root_bus = cyperus:get_root()

print(root_bus)
print(root_bus.ins)
print(dump(root_bus.ins))
print(root_bus.ins[1])
print(root_bus.full_path)

print(root_bus.outs)
print(dump(root_bus.outs))
print(root_bus.outs[1])
print(root_bus.full_path)


root_bus.ins[1]:connect()

freq=440
pw=0.5
amp=0.5
cutoff=800
gain=1
release=0.9

pulse = root_bus:add_module('audio/oscillator/pulse', freq, pw, 1.0, 0.0)
-- moogff = root_bus:add_module('audio/filter/moogff', cutoff, gain, 0.0, 1.0, 0.0)
-- perc = root_bus:add_module('motion/envelope/stdshape', 3, 0.01, release, 0.5, -4.0)

PARAM_MUL=3

-- print(pulse.outs)

-- perc.outs[1].connect(moogff.ins[PARAM_MUL])
-- pulse.outs[1].connect(moogff.ins[1])
-- moogff.outs[1].connect(root_bus.outs[1])
-- root_bus.outs[1].connect(cyperus.outs[1])

-- perc.gate=-1.0

print('end of lua script')