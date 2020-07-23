log = io.output("/sdcard/Android/data/com.popcap.pvz_na/scripts/LOG")
log:write(string.format("LUA SCRIPT LOADED, CORELIB %8x\n", getCoreLib()))
log:write(string.format("BASE %8x\n", getBase()))
log:write(string.format("STATUS %8x\n", getStatus()))
log:write(string.format("%x %x %x\n", putGrave(2, 2), putPlant(0, 2, 3), putZombie(1, 2, 4)))
for i, j in pairs(getPlants()) do
  log:write(string.format("%d %d\n", getI32(j + 0x1c), getI32(j + 0x34)))
end
log:close()
