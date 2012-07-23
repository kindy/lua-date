local date = require("lua-date")

--return interval days between two dates, result is 31
local days = date.days_diff("2011-12-22", "2012-01-01")
print(days)

-- return dates of table between two given dates
local date_table = date.days_between("2011-12-12", "2011-12-14")

-- date_table is {"2011-12-12", "2011-12-13", "2011-12-14"}
for k, v in ipairs(date_table) do
    print(v)
end

local date2 = date.format("2011-12-11")
print(date2)

local week = date.weekday("2012-07-28")
print("weekday: " .. week)

local res_date = date.days_by_num("2012-06-10", 4)
--expect {"2012-06-10", "2012-06-11", "2012-06-12", "2012-06-13"}
for k, v in ipairs(res_date) do
    print(v)
end


local w = date.check("0")

print(w)
if w == 0 then
    print("hello")
end


local d = date.date_add("2012-07-23", 10)
print(d)

