-- 植物逃兵
while getStatus() ~= 0 do
  for i,f in pairs(getPlants()) do
    if getI32(f+0xbc)<=10000 then
      setI32(20000,f+0xbc)
    end
    if getI32(f+0xbc)<=18000 and getI32(f+0xbc)>=17000 then
      setI32(20000,f+0xbc)
      setI32(getI32(f+0x8)-80,f+0x8)
    end
    if getI32(f+0x8)<=130 then
      setI32(getI32(f+0x14c)+256,f+0x14c)
    end
  end
  os.execute("sleep 5")
end
