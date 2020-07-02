while getStatus() ~= 0 do
  setI32(1000, getStatus() + 0x56bc)
end
