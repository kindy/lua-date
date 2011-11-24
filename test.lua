local date = require("lua-date")

-- return interval days between two dates, result is 31
local days = date.days_diff("2011-12-12", "2012-01-11")

-- return dates of table between two given dates
local date_table = date.days_between("2011-12-12", "2011-12-14")

-- date_table is {"2011-12-12", "2011-12-13", "2011-12-14"}
for k, v in ipairs(date_table) do
    print(v)
end
