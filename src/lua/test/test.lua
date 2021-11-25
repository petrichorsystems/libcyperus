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

freq=440
pw=0.5
amp=0.5
cutoff=800
gain=1
release=0.9

cyperus = Cyperus.new('5000', '127.0.0.1', '5001')
print(cyperus.ins)
print(dump(cyperus.ins))
print(cyperus.outs)
print(dump(cyperus.outs))
root_bus = cyperus:get_root()
pulse = root_bus:add_module('audio/oscillator/pulse', freq, pw, 1.0, 0.0)
moogff = root_bus:add_module('audio/filter/moogff', cutoff, gain, 0.0, 1.0, 0.0)
perc = root_bus:add_module('motion/envelope/stdshape', 3, 0.01, release, 0.5, -4.0)

param_mul=3
perc.outs[1]:connect(moogff.ins[param_mul])
pulse.outs[1]:connect(moogff.ins[1])
moogff.outs[1]:connect(root_bus.outs[1])
-- 'stereo' output
root_bus.outs[1]:connect(cyperus.outs[1])
root_bus.outs[1]:connect(cyperus.outs[2])

-- using a class member to provide a momentary gate to the percussive envelope
perc.gate=-1.0

-- create osc handler with user-specified path to supply a momentary gate
--   to the percussive envelope
-- perc.gate(

print('end of lua script')
