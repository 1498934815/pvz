function sleep(n)
  os.execute("sleep " .. n)
end
-- C类脚本要先sleep个10秒
sleep(10)
while true do
  if getStatus() ~= 0 then
    setI32(1, getStatus() + 0x56f8)
  end
  sleep(5)
end
