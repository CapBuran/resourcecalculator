function getIcon(object, size)
  local retval = ""
  if object.icon then
    retval = "{" .. object.icon .. ":"
    if object.icon_size then
      retval = retval .. tostring(object.icon_size)
    else
      retval = retval .. tostring(size)
    end 
    retval = retval .. "}"
  end
  return retval
end

function getIcons(object)
  local retval = ""
  local size = 0
  if object.icon_size then
    size = object.icon_size
  end
  if object.icon then
    retval = "{" .. getIcon(object, size) .. "}"
  elseif object.icons then
    retval = "{"
    for _, i in pairs(object.icons) do
      retval = retval .. getIcon(i)
    end
    retval = retval .. "}"
  end
  return retval
end

local entries = {
  "recipe",
  "inserter",
  "resource",
  "item",
  "fluid",
  "technology",
  "boiler",
  "generator",
  "reactor",
  "lab",
  "equipment",
  "assembling-machine",
  "furnace",
  "mining-drill",
  "transport-belt",
  "solar-panel",
  "equipment-grid"
}

for _, p in pairs(entries) do 
  if data.raw[p] then
    for _, i in pairs(data.raw[p]) do 
      log( "{" .. p .. "{" .. i.name .. "}" .. getIcons(i) .. "}")
    end
  end
end

--log(serpent.block(data.raw))
